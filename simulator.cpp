
#include "simulator.h"

#include <QList>
#include <QElapsedTimer>

#define FPSWINDOWSIZE 100

Simulator::Simulator(QWidget *parent) :
    QThread((QObject *)parent),
    maxticks(FPSWINDOWSIZE),
    tickwnd(new QList<qint64>()),
    ticks(new QElapsedTimer()),
    nselapsed(0)
{}

Simulator::~Simulator()
{
    delete tickwnd;
    delete ticks;
}

void Simulator::stop()
{
    doSimulate = false;
}

void Simulator::run()
{
    doSimulate = true;
    initialize();
    while(doSimulate) {
        updateFPS();
    }
}

void Simulator::initialize()
{
    ticks->start();
}

void Simulator::updateFPS()
{
    qint64 nsecs = ticks->nsecsElapsed();
    nselapsed += nsecs;
    ticks->start();
    if(tickwnd->count() > maxticks) {
        nselapsed -= (*tickwnd)[0];
        tickwnd->removeFirst();
    }
    tickwnd->append(nsecs);
    float fps = tickwnd->count() * 1000000000.0f / nselapsed;
//    printf("fps: %.4f\n", fps);
}
