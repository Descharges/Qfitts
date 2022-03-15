#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QRect scr = a.screens()[0]->geometry();
    int height = scr.height();
    int width = scr.width();
    MainWindow w(nullptr, width, height);
    w.show();
    return a.exec();
}
