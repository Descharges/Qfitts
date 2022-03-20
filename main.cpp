#include "mainwindow.h"
#include "maintest.h"

#include <QApplication>
#include <QSizePolicy>
#include <QLayout>
#include <QLineEdit>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QRect scr = a.screens()[0]->geometry();
    int height = scr.height();
    int width = scr.width();

    MainWindow w(nullptr,1500,1500);

    mainTest test(&w);

    w.show();
    return a.exec();
}
