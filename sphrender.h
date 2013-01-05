#ifndef SPHRENDER_H
#define SPHRENDER_H

#include <QGLWidget>

#include "simulator.h"

class QTimer;
class Simulator;

class SPHrender : public QGLWidget
{
    Q_OBJECT
public:
    explicit SPHrender(QWidget *parent = 0);
    virtual ~SPHrender();

public slots:
    void setFramerate(double fps);
    void setGravity(double G);
    void setTimestep(double deltaT);
    void stopSimulation();
    void startSimulation();
    void restartSimulation();

private:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    Simulator sph;
    QTimer *timer;

    float fps;
};

#endif // SPHRENDER_H
