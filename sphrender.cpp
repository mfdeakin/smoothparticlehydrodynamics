#include "sphrender.h"

SPHrender::SPHrender(QWidget *parent) :
    QGLWidget(parent), rendert(this)
{}

SPHrender::~SPHrender()
{
    /* Don't try to kill the thread while it's running */
    rendert.stop();
    rendert.wait();
}

void SPHrender::initializeGL()
{
    /* Hand the reins off to the other thread */
    doneCurrent();
    rendert.start();
}

void SPHrender::resizeEvent(QResizeEvent *evt)
{
    rendert.resize(evt->size());
    if(!rendert.isRunning())
        /* Qt will try to use the OpenGL context,
         * but we don't want that. However, Qt
         * will not call initializeGL without this,
         * so let it do its stuff until OpenGL
         * is initialized */
        QGLWidget::resizeEvent(evt);
}

void SPHrender::paintEvent(QPaintEvent *evt)
{
    /* Qt will try to use the OpenGL context,
     * but we don't want that. */
}
