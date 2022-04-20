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

//Initialise the window, put the elements in place and define their properties
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
    QPushButton *startBtn2;
    QPushButton *resultBtn;
    QPushButton *backBtn;

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

    QCheckBox *themeSelect;

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
    QPushButton *simulationBtn;
    QPushButton *quitBtn;
    QPushButton *graph1Btn;
    QPushButton *graph2Btn;

    QHBoxLayout *titleBtnLayout;

    friend FittsController;
    friend FittsModel;

    QLabel *ratio = 0;
    QLabel *failedClicks = 0;

    QString theme;
private slots:
    void setTheme(int theme);
};

#endif // MAINWINDOW_H
