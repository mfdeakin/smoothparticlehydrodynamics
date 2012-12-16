#include "sphwindow.h"
#include "ui_sphwindow.h"

SPHWindow::SPHWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SPHWindow)
{
    ui->setupUi(this);
}

SPHWindow::~SPHWindow()
{
    delete ui;
}
