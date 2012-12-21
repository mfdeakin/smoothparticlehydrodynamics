#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QThread>
#include <QList>
#include <QElapsedTimer>
#include <QSize>

class Simulator : public QThread
{
    Q_OBJECT
public:
    explicit Simulator(QWidget *parent = 0);
    void initialize();
    void run();
    void render();
    void stop();
signals:
    
public slots:
    
private:
    void updateFPS();

    bool doSimulate;

    unsigned maxticks;
    QList<qint64> tickwnd;
    QElapsedTimer ticks;
    qint64 nselapsed;
};

#endif // RENDERTHREAD_H
