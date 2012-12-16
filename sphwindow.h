#ifndef SPHWINDOW_H
#define SPHWINDOW_H

#include <QMainWindow>

namespace Ui {
class SPHWindow;
}

class SPHWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit SPHWindow(QWidget *parent = 0);
    ~SPHWindow();
    
private:
    Ui::SPHWindow *ui;
};

#endif // SPHWINDOW_H
