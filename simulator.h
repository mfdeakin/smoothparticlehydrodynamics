#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QThread>

template<typename t> class QList;
class QElapsedTimer;
class QMutex;
struct Particle;
struct Vector;

class Simulator : public QThread
{
    Q_OBJECT
public:
    explicit Simulator(QWidget *parent = 0);
    virtual ~Simulator();
    void initialize();
    void stop();
    virtual void render();

    void setGravitationalConstant(float G);
    void setTimestep(float deltaT);

    virtual void restartSimulation();
signals:
    
public slots:
    
protected:
    virtual void simulate();
    virtual float *calcCOM();
    virtual struct Particle *initParticles(unsigned count);

private:
    void run();
    void updateFPS();

    bool doSimulate;
    float totalmass;
    float timestep;
    float framerate;
    float gconst;

    struct Particle *particles,
            *particleBuf;
    unsigned particleCnt;

    unsigned maxticks;
    QList<qint64> *tickwnd;
    QElapsedTimer *ticks;
    qint64 nselapsed;
    QMutex *particleMtx;
};

#endif // RENDERTHREAD_H
