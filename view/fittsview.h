#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphicwidget.h"

#include <QMainWindow>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QStackedLayout>
#include <QGraphicsView>
#include <QLabel>
#include <QLineSeries>
#include <QtCharts>
#include <QHBoxLayout>

QT_CHARTS_USE_NAMESPACE

class FittsModel;
class FittsController;

class FittsView : public QMainWindow
{
    Q_OBJECT
public:
    FittsView(FittsModel *fittsModel, FittsController *fittsController);
    ~FittsView();

    void initWindows();

private:
    void updateTestMsg();
    void displayResults();

    FittsModel *fittsModel;
    FittsController *fittsController;

    QPushButton *startBtn;
    QPushButton *resultBtn;
    QPushButton *resultLeaveBtn;

    QSlider *aValue;
    QSlider *bValue;
    QSlider *nbCible;
    QSlider *minSize;
    QSlider *maxSize;
    QLabel *aValueLabel;
    QLabel *bValueLabel;
    QLabel *nbCibleLabel;
    QLabel *minSizeLabel;
    QLabel *maxSizeLabel;

    QStackedLayout *mainStack;
    QStackedLayout *graphStack;

    GraphicWidget *graphicView;
    QGraphicsScene *scene;

    QLabel *testLabel;
    QLabel *ecartType;
    QLabel *erreurType;
    QLabel *diffMoy;
    QLabel *itc95;

    QChartView *plot;
    QChartView *plot2;

    QPushButton *settingBtn;
    QPushButton *statBtn;
    QPushButton *graph1;
    QPushButton *graph2;

    QHBoxLayout *titleBtnLayout;

    friend FittsController;
    friend FittsModel;

    QLabel *ratio = 0;
    QLabel *failedClicks = 0;
};

#endif // MAINWINDOW_H