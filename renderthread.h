#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QThread>
#include <QList>
#include <QElapsedTimer>
#include <QSize>

class RenderThread : public QThread
{
    Q_OBJECT
public:
    explicit RenderThread(QWidget *parent = 0);
    void resize(QSize sz);
    void initialize();
    void run();
    void stop();
signals:
    
public slots:
    
private:
    void paintGL();
    void updateFPS();
    void updateViewport();

    bool doRender;
    bool doResize;

    QSize size;

    unsigned maxticks;
    QList<qint64> tickwnd;
    QElapsedTimer ticks;
    qint64 nselapsed;
};

#endif // RENDERTHREAD_H
