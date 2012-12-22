#include "sphrender.h"
#include <assert.h>
#include <QtOpenGL>
#include <QTimer>

/* Factory for the format needed for rendering */
QGLFormat createFormat();

SPHrender::SPHrender(QWidget *parent) :
    QGLWidget(createFormat(), parent),
    sph(this), timer(new QTimer(this)), fps(60)
{
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
}

SPHrender::~SPHrender()
{
    /* Don't try to kill the thread while it's running */
    sph.stop();
    sph.wait();
}

void SPHrender::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5, 0.5, -0.5, 0.5, 0.1, 10.0);

    setFramerate(fps);
    sph.start();
}

void SPHrender::paintGL()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    static int angle = 0;
    angle = (angle + 1) % 360;
    glRotatef(angle, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT |
            GL_STENCIL_BUFFER_BIT);
    /* Paint the background */
    glClear(GL_DEPTH_BUFFER_BIT);
    /* Paint the foreground */
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(1.0, 0.0, 0.0); glVertex3f(-0.5, -0.5, -1.0);
    glColor3f(0.0, 1.0, 0.0); glVertex3f(-0.5, +0.5, -1.0);
    glColor3f(0.0, 0.0, 1.0); glVertex3f(+0.5, -0.5, -1.0);
    glEnd();
}

void SPHrender::resizeGL(int w, int h)
{
    int length = w < h ? w : h;
    glViewport((w - length) / 2,
               (h - length) / 2,
               length, length);
}

void SPHrender::setFramerate(double fps)
{
    /* Calculate the number of milliseconds between each frame */
    this->fps = fps;
    timer->stop();
    timer->start(1000.0 / fps);
}

QGLFormat createFormat()
{
    /* The format needs to be set up properly in *
     * QGLWidgets constructor, so generate it here */
    QGLFormat fmt(QGL::DoubleBuffer |
                  QGL::DepthBuffer |
                  QGL::StencilBuffer |
                  QGL::DirectRendering);
    fmt.setSwapInterval(0);
    return fmt;
}
