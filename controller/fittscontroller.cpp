#include "fittscontroller.h"
#include "fittsview.h"
#include "fittsmodel.h"

#include <QRandomGenerator>
#include <QApplication>
#include <QDebug>
#include <QtGlobal>
#include <QChart>
#include <QLineSeries>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QLegend>
#include <QBarCategoryAxis>
#include <QHorizontalBarSeries>
#include <QCategoryAxis>
#include <QPieSeries>
#include <QPieSlice>
#include <QStringList>
#include <QElapsedTimer>
#include <QVector>

QT_CHARTS_USE_NAMESPACE

FittsController::FittsController() {
    this->fittsModel =new FittsModel(this);
    this->start();
}

void FittsController::start() {
    this->startSimulation();
    this->fittsView->show();

}

void FittsController::startSimulation() {
    this->fittsView->mainStack->setCurrentIndex(1);
    this->fittsModel->cibleLeft = this->fittsModel->nbCible;
    this->fittsView->updateTestMsg();
    this->fittsView->resultBtn->setEnabled(false);
    this->fittsView->graphicView->setEnabled(true);
    this->fittsModel->cercleSize.clear();
    this->fittsModel->cercleCenter.clear();
    this->fittsModel->clickPoints.clear();
    this->fittsModel->times.clear();



    this->initGame();
}

void FittsController::quit() {
    QApplication::quit();
}




void FittsController::cancel() {
    this->fittsView->mainStack->setCurrentIndex(0);
}


void FittsController::aValueChanged(int value) {
    float valuef = value;
    valuef /= 10;
    this->fittsModel->a = valuef;
    this->fittsView->aValueLabel->setText(QString::number(valuef));
}
void FittsController::bValueChanged(int value) {
    float valuef = value;
    valuef /= 10;
    this->fittsModel->b = valuef;
    this->fittsView->bValueLabel->setText(QString::number(valuef));
}
void FittsController::nbCibleChanged(int value) {
    this->fittsModel->nbCible = value;
    this->fittsView->nbCibleLabel->setText(QString::number(value));
}
void FittsController::minSizeChanged(int value) {
    this->fittsModel->minSize = value;
    this->fittsView->minSizeLabel->setText(QString::number(value));
    this->fittsView->maxSize->setMinimum(value+5);
}
void FittsController::maxSizeChanged(int value) {
    this->fittsModel->maxSize = value;
    this->fittsView->maxSizeLabel->setText(QString::number(value));
    this->fittsView->minSize->setMaximum(value-5);
}

void FittsController::toGraph1(){
    this->fittsView->graphStack->setCurrentIndex(0);
}
void FittsController::toGraph2(){
    this->fittsView->graphStack->setCurrentIndex(1);
}
void FittsController::toSettings(){
    this->fittsView->mainStack->setCurrentIndex(0);
}
void FittsController::toStats(){
    this->fittsView->mainStack->setCurrentIndex(2);
}



void FittsController::cibleClicked(int x, int y) {
    if(this->fittsModel->cercleCenter.isEmpty()) {
        // Si vide alors premier click, on demarre le timer
        this->timer = new QElapsedTimer;
        timer->start();

        // On démarre avec la première cible
        this->fittsModel->clickPoints.append(QPoint(x,y));
        this->nextCible();
    }
    else {
        QPointF coords = this->fittsView->graphicView->mapToScene(x,y);
        if(sqrt(pow(coords.x() - this->fittsModel->cercleCenter.last().x(),2) + pow(coords.y() - this->fittsModel->cercleCenter.last().y(),2)) <= this->fittsModel->cercleSize.last() / 2) {
            this->fittsModel->clicks ++;
            // On stock le temps de click
            this->fittsModel->times.append(timer->elapsed());
            // On restart le chrono
            timer->restart();

            // On stock la position du click
            this->fittsModel->clickPoints.append(QPoint(x,y));
            this->nextCible();
        }
        else
        {
            this->fittsModel->failedClicks ++;
        }
    }
}

void FittsController::nextCible() {
    if(!this->fittsModel->cercleCenter.isEmpty())
        this->fittsModel->cibleLeft--;
    this->fittsView->updateTestMsg();

    QGraphicsScene *scene = this->fittsView->scene;
    scene->clear();

    // On stop si c'est finis
    if(this->fittsModel->cibleLeft == 0) {
        this->finish();
        return;
    }

    // On génère la taille du cercle rouge
    // qrand() % ((high + 1) - low) + low;
    int size =  QRandomGenerator::global()->bounded(((this->fittsModel->maxSize + 1) - this->fittsModel->minSize))  + this->fittsModel->minSize;
    // Car on veut le rayon
    // On place le cercle dans la scene (Attention faut pas qu'il soit en dehors du cadre)
    int sceneW = int(this->fittsView->scene->width());
    int sceneH = int(this->fittsView->scene->height());

    double posX =  QRandomGenerator::global()->bounded(((sceneW - size) - size)) + size ;
    double posY =  QRandomGenerator::global()->bounded(((sceneH - size) - size)) + size ;

    // On stock les infos sur le cercle
    this->fittsModel->cercleCenter.append(QPoint(int(posX),int(posY)));
    this->fittsModel->cercleSize.append(size);

    // On place le cercle
    scene->addEllipse(posX - (size / 2), posY - (size / 2), size, size, QPen(QColor("red")),QBrush(QColor("red")));
}


void FittsController::finish() {


    this->fittsView->mainStack->setCurrentIndex(2);
    this->fittsView->graphStack->setCurrentIndex(0);
    this->fittsModel->calculateResult();

    this->fittsView->settingBtn->setVisible(true);
    this->fittsView->statBtn->setVisible(true);
}

void FittsController::initGame() {
    QGraphicsScene *scene = this->fittsModel->fittsView->scene;
    scene->clear();

    if(this->fittsModel->maxSize >= this->fittsView->graphicView->width() / 2)
        this->fittsModel->maxSize = this->fittsView->graphicView->width() / 2;

    if(this->fittsModel->maxSize >= this->fittsView->graphicView->height() / 2)
        this->fittsModel->maxSize = this->fittsView->graphicView->height() / 2;

    qreal posX = scene->width() / 2;
    qreal posY = scene->height() / 2;
    int size = 100;

    scene->addEllipse(posX - (size / 2), posY - (size / 2), size, size, QPen(QColor("blue")),QBrush(QColor("blue")));
}
