#ifndef SPHRENDER_H
#define SPHRENDER_H

#include <QGLWidget>
#include <QtOpenGL>

#include "renderthread.h"

class RenderThread;

class SPHrender : public QGLWidget
{
    Q_OBJECT
public:
    explicit SPHrender(QWidget *parent = 0);
    virtual ~SPHrender();

private:
    void initializeGL();
    void resizeEvent(QResizeEvent *evt);
    void paintEvent(QPaintEvent *evt);

    RenderThread rendert;
};

#endif // SPHRENDER_H
