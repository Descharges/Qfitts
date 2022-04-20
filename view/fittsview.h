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
    QPushButton *resultBtn;
    QPushButton *resultLeaveBtn;
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
    QPushButton *graph1;
    QPushButton *graph2;

    QHBoxLayout *titleBtnLayout;

    friend FittsController;
    friend FittsModel;

    QLabel *ratio = 0;
    QLabel *failedClicks = 0;

    QString theme ="\
            QMainWindow {\
                background-color: lightblue;\
            }\
            QPushButton {\
                color: white; background-color: #2596be; border-radius: 5px;\
            }\
            QPushButton#set {\
                color: white; background-color: #2596be; border-radius: 25px;\
            }\
            QPushButton#stat {\
                color: white; background-color: #2596be; border-radius: 25px;\
            }\
            QPushButton:hover {\
                background-color: red;\
            }\
            QGroupBox {\
                background-color: #2596be;\
                color: white;\
                font-size: 15px;\
                font-weight: bold;\
                border-radius: 10px;\
                padding: 20px\
            }\
            QLabel {\
                background-color: #2596be;\
                color: white;\
            }\
            QSlider {\
                background-color: #2596be;\
                color: white;\
            }\
            QSlider::groove:horizontal {\
                height: 10px;\
                border-radius: 5px;\
                background-color: white;\
                margin: 0px;\
            }\
            QSlider::handle:horizontal {\
                background-color: black;\
                border-radius: 5px;\
                height: 10px;\
                width: 10px;\
            }\
            ";
private slots:
    void setTheme(int theme);
};

#endif // MAINWINDOW_H
