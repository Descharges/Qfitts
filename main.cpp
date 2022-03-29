#include "controller/fittscontroller.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);

    FittsController *fittsController = new FittsController;

    return a.exec();
}
