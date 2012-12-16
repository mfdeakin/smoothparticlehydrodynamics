#include <QtGui/QApplication>
#include "sphwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SPHWindow w;
    w.show();
    
    return a.exec();
}
