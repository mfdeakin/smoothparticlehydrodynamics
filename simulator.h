#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QThread>

template<typename t> class QList;
class QElapsedTimer;

class Simulator : public QThread
{
    Q_OBJECT
public:
    explicit Simulator(QWidget *parent = 0);
    virtual ~Simulator();
    void initialize();
    void stop();
signals:
    
public slots:
    
protected:
    virtual void simulate();

private:
    void run();
    void updateFPS();

    bool doSimulate;

    unsigned maxticks;
    QList<qint64> *tickwnd;
    QElapsedTimer *ticks;
    qint64 nselapsed;
};

#endif // RENDERTHREAD_H
