
#include "renderthread.h"
#include "sphrender.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

RenderThread::RenderThread(QWidget *parent) :
    QThread(parent), tickwnd(), ticks(),
    nselapsed(0), maxticks(10)
{
}

void RenderThread::stop()
{
    doRender = false;
}

void RenderThread::run()
{
    doRender = true;
    doResize = true;
    initialize();
    while(doRender) {
        if(doResize)
            updateViewport();
        paintGL();
        updateFPS();
    }
    ((SPHrender *)this->parent())->doneCurrent();
}

void RenderThread::initialize()
{

    ((SPHrender *)this->parent())->makeCurrent();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5f, 0.5f, -0.5f, 0.5f, 0.1f, 10.0f);

    ticks.start();
}

void RenderThread::updateViewport()
{
    int side = size.width() < size.height() ?
                size.width() : size.height();
    glViewport((size.width() - side) / 2,
               (size.height() - side) / 2,
               side, side);
    doResize = false;
}

void RenderThread::resize(QSize sz)
{
    size = sz;
    doResize = true;
}

void RenderThread::updateFPS()
{
    qint64 nsecs = ticks.nsecsElapsed();
    nselapsed += nsecs;
    ticks.start();
    if(tickwnd.count() > maxticks) {
        nselapsed -= tickwnd[0];
        tickwnd.removeFirst();
    }
    tickwnd.append(nsecs);
    float fps = tickwnd.count() * 1000000000.0f / nselapsed;
}

void RenderThread::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    static float angle;
    angle += 0.1f;
    glColor3f(1.0f, 1.0f, 1.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);

    glBegin(GL_LINES);
    glVertex3f(-0.5f, 0.0f, -5.0f);
    glVertex3f(0.5f, 0.0f, -5.0f);
    glVertex3f(0.0f, -0.5f, -5.0f);
    glVertex3f(0.0f, 0.5f, -5.0f);
    glEnd();

    glFlush();
    ((SPHrender *)parent())->swapBuffers();
}
