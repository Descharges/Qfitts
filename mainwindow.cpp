#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent, int screenW, int screenH)
    : QMainWindow(parent)
{
    this->setGeometry(screenW/4,screenH/4,screenW/2,screenH/2);
    this->setFixedSize(screenW/2,screenH/2);
}

MainWindow::~MainWindow()
{
}

