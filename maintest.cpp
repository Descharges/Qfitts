#include "maintest.h"
#include <QSizePolicy>

mainTest::mainTest(QWidget *parent)
    : QWidget{parent}
{
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, Qt::red);

    this->setAutoFillBackground(true);
    this->setPalette(pal);
    QRect geo = this->parentWidget()->geometry();
    this->setGeometry(QRect(50,10, geo.width()-60,geo.height()-20));
}
