
#include "simulator.h"
#include "sphrender.h"

Simulator::Simulator(QWidget *parent) :
    QThread(parent), tickwnd(), ticks(),
    nselapsed(0), maxticks(10)
{}

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
    ticks.start();
}

void Simulator::updateFPS()
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
//    printf("fps: %.4f\n", fps);
}

void Simulator::render()
{

}
