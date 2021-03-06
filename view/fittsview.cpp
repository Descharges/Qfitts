#include "fittsview.h"
#include "graphicwidget.h"
#include "fittscontroller.h"
#include "fittsmodel.h"

#include <iostream>
#include <string>
#include <QTextStream>
#include <QDebug>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>

#include <QStackedLayout>
#include <QGraphicsView>
#include <QChart>
#include <QPolarChart>
#include <QLineSeries>

QT_CHARTS_USE_NAMESPACE

FittsView::FittsView(FittsModel *fittsModel, FittsController *fittsController) : QMainWindow() {
    this->fittsModel = fittsModel;
    this->fittsController = fittsController;
    this->fittsController->fittsView = this;

    this->initWindows();

    // Btn clicked

    connect(themeSelect, SIGNAL(stateChanged(int)), this, SLOT(setTheme(int)));


    connect(backBtn,SIGNAL(clicked()),fittsController,SLOT(cancel()));
    connect(importXmlBtn,SIGNAL(clicked()),fittsController, SLOT(importXml()));
    connect(exportXmlBtn,SIGNAL(clicked()),fittsController, SLOT(exportXml()));


    connect(graphicView, SIGNAL(mouseClicked(int,int)), fittsController, SLOT(cibleClicked(int,int)));
    connect(graph1Btn, SIGNAL(clicked()), fittsController, SLOT(toGraph1()));
    connect(graph2Btn, SIGNAL(clicked()), fittsController, SLOT(toGraph2()));

    connect(settingBtn, SIGNAL(clicked()), fittsController, SLOT(toSettings()));
    connect(statBtn, SIGNAL(clicked()), fittsController, SLOT(toStats()));
    connect(simulationBtn, SIGNAL(clicked()), fittsController, SLOT(startSimulation()));
    connect(quitBtn, SIGNAL(clicked()), fittsController, SLOT(quit()));

    // SpinBox values update
    connect(aValue,SIGNAL(valueChanged(int)),fittsController,SLOT(aValueChanged(int)));
    connect(bValue,SIGNAL(valueChanged(int)),fittsController,SLOT(bValueChanged(int)));
    connect(nbCible,SIGNAL(valueChanged(int)),fittsController,SLOT(nbCibleChanged(int)));
    connect(minSize,SIGNAL(valueChanged(int)),fittsController,SLOT(minSizeChanged(int)));
    connect(maxSize,SIGNAL(valueChanged(int)),fittsController,SLOT(maxSizeChanged(int)));
}

FittsView::~FittsView() {}

void FittsView::initWindows() {



    //window

    this->set = new QSettings(QSettings::IniFormat, QSettings::UserScope,"utbm","Qfitts");
    if( this->set->value("set/darkmode") == "1"){
        setTheme(1);
    }else{
        setTheme(0);
    }

    QLabel *label;


    QVBoxLayout *titleBtnLayout = new QVBoxLayout;


    simulationBtn = new QPushButton("");
    simulationBtn->setFixedSize(50,50);
    simulationBtn->setObjectName("menu");
    //simulationBtn->setStyleSheet(this->theme);
    simulationBtn->setIcon(QIcon(QDir::currentPath() + "/curs.png"));
    simulationBtn->setIconSize(QSize(40, 40));
    simulationBtn->setCursor(Qt::PointingHandCursor);
    titleBtnLayout->addWidget(simulationBtn);

    settingBtn = new QPushButton("");
    settingBtn->setFixedSize(50,50);
    settingBtn->setObjectName("menu");
    //settingBtn->setStyleSheet(this->theme);
    settingBtn->setIcon(QIcon(QDir::currentPath() + "/set.png"));
    settingBtn->setIconSize(QSize(30, 30));
    settingBtn->setCursor(Qt::PointingHandCursor);
    titleBtnLayout->addWidget(settingBtn);

    statBtn = new QPushButton("");
    statBtn->setFixedSize(50,50);
    statBtn->setObjectName("menu");
    //statBtn->setStyleSheet(this->theme);
    statBtn->setIcon(QIcon(QDir::currentPath() + "/stat.png"));
    statBtn->setIconSize(QSize(40, 40));
    statBtn->setCursor(Qt::PointingHandCursor);
    titleBtnLayout->addWidget(statBtn);

    titleBtnLayout->addStretch();

    quitBtn = new QPushButton("");
    quitBtn->setFixedSize(50,50);
    quitBtn->setObjectName("menu");
    //quitBtn->setStyleSheet(this->theme);
    quitBtn->setIcon(QIcon(QDir::currentPath() + "/quit.png"));
    quitBtn->setIconSize(QSize(40, 40));
    quitBtn->setCursor(Qt::PointingHandCursor);
    titleBtnLayout->addWidget(quitBtn);




    QVBoxLayout *titles = new QVBoxLayout;

    titles->addLayout(titleBtnLayout);




    QWidget *mainWidget = new QWidget;
    //mainWidget->setStyleSheet(this->theme);
    this->setCentralWidget(mainWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(mainWidget);

    mainLayout->addLayout(titles);

    mainStack = new QStackedLayout;
    mainLayout->addLayout(mainStack);




    //setting

    QWidget *settingsWidget = new QWidget;
    mainStack->addWidget(settingsWidget);


    QVBoxLayout *settingsLayout = new QVBoxLayout(settingsWidget);


    QGroupBox *rappelBox = new QGroupBox(" Param??tre de l'application :");
    settingsLayout->addWidget(rappelBox);
    QHBoxLayout *rappelLayout = new QHBoxLayout(rappelBox);

    this->themeSelect = new QCheckBox;
    rappelLayout->addWidget(new QLabel("Th??me sombre:"));
    rappelLayout->addWidget(this->themeSelect);
    if( this->set->value("set/darkmode") == "1"){
        this->themeSelect->setChecked(true);
    }else{
        this->themeSelect->setChecked(false);
    }
    rappelLayout->addStretch();


    QGroupBox *configBox = new QGroupBox(" Configuration du test :");
    settingsLayout->addWidget(configBox);
    QGridLayout *configLayout = new QGridLayout(configBox);

    configLayout->addWidget(new QLabel("Variable a :"),1,0);
    configLayout->addWidget(new QLabel("Variable b :"),2,0);
    configLayout->addWidget(new QLabel("Nombre de cibles:"),3,0);
    configLayout->addWidget(new QLabel("Taille minimum cible:"),4,0);
    configLayout->addWidget(new QLabel("Taille maximal cible:"),5,0);

    aValueLabel = new QLabel;
    bValueLabel = new QLabel;
    nbCibleLabel = new QLabel;
    minSizeLabel = new QLabel;
    maxSizeLabel = new QLabel;

    this->aValueLabel->setText(QString::number(this->fittsModel->a));
    this->bValueLabel->setText(QString::number(this->fittsModel->b));
    this->nbCibleLabel->setText(QString::number(this->fittsModel->nbCible));
    this->minSizeLabel->setText(QString::number(this->fittsModel->minSize));
    this->maxSizeLabel->setText(QString::number(this->fittsModel->maxSize));

    configLayout->addWidget(aValueLabel,1,2);
    configLayout->addWidget(bValueLabel,2,2);
    configLayout->addWidget(nbCibleLabel,3,2);
    configLayout->addWidget(minSizeLabel,4,2);
    configLayout->addWidget(maxSizeLabel,5,2);

    aValue = new QSlider(Qt::Horizontal);
    aValue->setMinimum(0);
    aValue->setMaximum(50);
    aValue->setValue((this->fittsModel->a));
    aValue->setMaximumSize(200,50);
    aValue->setFocusPolicy(Qt::StrongFocus);
    aValue->setTickPosition(QSlider::TicksAbove);
    aValue->setTickInterval(10);
    aValue->setSingleStep(1);
    configLayout->addWidget(aValue,1,1);

    bValue = new QSlider(Qt::Horizontal);
    bValue->setMinimum((this->fittsModel->b) * 10);
    bValue->setMaximum(100);
    bValue->setValue(this->fittsModel->b);
    bValue->setMaximumSize(200,50);
    bValue->setFocusPolicy(Qt::StrongFocus);
    bValue->setTickPosition(QSlider::TicksAbove);
    bValue->setTickInterval(10);
    bValue->setSingleStep(1);
    configLayout->addWidget(bValue,2,1);

    nbCible = new QSlider(Qt::Horizontal);
    nbCible->setMinimum(2);
    nbCible->setMaximum(50);
    nbCible->setValue(this->fittsModel->nbCible);
    nbCible->setMaximumSize(200,50);
    nbCible->setFocusPolicy(Qt::StrongFocus);
    nbCible->setTickPosition(QSlider::TicksAbove);
    nbCible->setTickInterval(10);
    nbCible->setSingleStep(1);
    configLayout->addWidget(nbCible,3,1);

    minSize = new QSlider(Qt::Horizontal);
    minSize->setMaximum(175);
    minSize->setMinimum(10);
    minSize->setValue(this->fittsModel->minSize);
    minSize->setMaximumSize(200,50);
    minSize->setFocusPolicy(Qt::StrongFocus);
    minSize->setTickPosition(QSlider::TicksAbove);
    minSize->setTickInterval(10);
    minSize->setSingleStep(1);
    configLayout->addWidget(minSize,4,1);

    maxSize = new QSlider(Qt::Horizontal);
    maxSize->setMaximum(180);
    maxSize->setMinimum(15);
    maxSize->setValue(this->fittsModel->maxSize);
    maxSize->setMaximumSize(200,50);
    maxSize->setFocusPolicy(Qt::StrongFocus);
    maxSize->setTickPosition(QSlider::TicksAbove);
    maxSize->setTickInterval(10);
    maxSize->setSingleStep(1);
    configLayout->addWidget(maxSize,5,1);


    QHBoxLayout *btnLayout = new QHBoxLayout;
    settingsLayout->addLayout(btnLayout);


    // Test part

    QWidget *testWidget = new QWidget;
    mainStack->addWidget(testWidget);

    QVBoxLayout *testLayout = new QVBoxLayout(testWidget);

    testLabel = new QLabel;
    testLayout->addWidget(testLabel);

    graphicView = new GraphicWidget;
    testLayout->addWidget(graphicView);
    graphicView->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff);
    graphicView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scene = new QGraphicsScene;
    graphicView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    graphicView->setScene(scene);
    scene->setSceneRect(0,0,graphicView->width(),300);

    label = new QLabel("<strong>Le test commencera apr??s avoir appuy?? sur la cible <font color='blue'><strong>bleu</strong></font></strong>.<br/>Cliquez sur les cibles qui apparaissent en <font color='red'><strong>rouge</strong></font>. ");
    label->setStyleSheet("QLabel{font-size: 18pt;}");
    label->setAlignment(Qt::AlignCenter);
    testLayout->addWidget(label);

    btnLayout = new QHBoxLayout;
    testLayout->addLayout(btnLayout);

    resultBtn = new QPushButton("R??sultats");

    backBtn = new QPushButton("Annuler");

    //graphic

    QWidget *resultWidget = new QWidget;

    mainStack->addWidget(resultWidget);

    QHBoxLayout *masterResLay = new QHBoxLayout(resultWidget);

    QVBoxLayout *resultLayout = new QVBoxLayout;

    QHBoxLayout *statBtnLayout = new QHBoxLayout;


    masterResLay->addLayout(resultLayout);


    graphStack = new QStackedLayout;
    resultLayout->addLayout(graphStack);


    graph1Btn = new QPushButton("Graphique T = f(D)");
    graph1Btn->setCursor(Qt::PointingHandCursor);
    graph1Btn->setMinimumHeight(30);
    statBtnLayout->addWidget(graph1Btn);

    graph2Btn = new QPushButton("Graphique T = log(2D/L)");
    graph2Btn->setCursor(Qt::PointingHandCursor);
    graph2Btn->setMinimumHeight(30);
    statBtnLayout->addWidget(graph2Btn);

    resultLayout->addLayout(statBtnLayout);

    plot = new QChartView;
    plot->setMinimumHeight(300);
    plot->setRubberBand(QChartView::HorizontalRubberBand);
    graphStack->addWidget(plot);

    plot2 = new QChartView;
     plot->setMinimumHeight(300);
    plot2->setRubberBand(QChartView::HorizontalRubberBand);
    graphStack->addWidget(plot2);

    QGroupBox *resultBox =  new QGroupBox("  Statistiques");
    resultLayout->addWidget(resultBox);
    QGridLayout *resultBoxLayout = new QGridLayout(resultBox);

    label = new QLabel("Ecart-Type : ");
    resultBoxLayout->addWidget(label,0,0);
    ecartType = new QLabel;
    resultBoxLayout->addWidget(ecartType,0,1);

    label = new QLabel("Erreur-Type : ");
    resultBoxLayout->addWidget(label,1,0);
    erreurType = new QLabel;
    resultBoxLayout->addWidget(erreurType,1,1);

    label = new QLabel("Diff??rence moyenne : ");
    resultBoxLayout->addWidget(label,0,2);
    diffMoy = new QLabel;
    resultBoxLayout->addWidget(diffMoy,0,3);

    label = new QLabel("Intervalle de confiance ?? 95% : ");
    resultBoxLayout->addWidget(label,1,2);
    itc95 = new QLabel;
    resultBoxLayout->addWidget(itc95,1,3);

    label = new QLabel("Cliques rat??s : ");
    resultBoxLayout->addWidget(label,0,4);
    failedClicks = new QLabel;
    resultBoxLayout->addWidget(failedClicks,0,5);

    label = new QLabel("Ratio : ");
    resultBoxLayout->addWidget(label,1,4);
    ratio = new QLabel;
    resultBoxLayout->addWidget(ratio,1,5);

    resultBoxLayout->setColumnStretch(1,10);
    resultBoxLayout->setColumnStretch(3,10);

    btnLayout = new QHBoxLayout;
    resultLayout->addLayout(btnLayout);



    exportXmlBtn = new QPushButton("Exporter la simulation");
    exportXmlBtn->setMinimumHeight(30);
    exportXmlBtn->setCursor(Qt::PointingHandCursor);

    importXmlBtn = new QPushButton("Importer une simulation");
    importXmlBtn->setMinimumHeight(30);
    importXmlBtn->setCursor(Qt::PointingHandCursor);

    btnLayout->addWidget(exportXmlBtn);
    btnLayout->addWidget(importXmlBtn);

}

void FittsView::updateTestMsg() {
    this->testLabel->setText("Nombre de cibles restantes : " + QString::number(this->fittsModel->cibleLeft));
    this->testLabel->setStyleSheet("QLabel{font-size: 18pt;}");
    this->testLabel->setAlignment(Qt::AlignCenter);
}


void FittsView::displayResults() {
    diffMoy->setText(QString::number(fittsModel->diffMoy));
    ecartType->setText(QString::number(fittsModel->ecartType));
    erreurType->setText(QString::number(fittsModel->erreurType));
    itc95->setText(QString::number(fittsModel->itc95));
    failedClicks->setText(QString::number(fittsModel->failedClicks));
    ratio->setText(QString::number(fittsModel->ratio));
}

void FittsView::setTheme(int theme){
    if(theme==0){//White mode
        this->set->setValue("set/darkmode",0);
        this->theme ="\
                QMainWindow {\
                    background-color: lightblue;\
                }\
                QPushButton {\
                    color: white; background-color: #2596be; border-radius: 5px;\
                }\
                QLabel#save {\
                    color: white; background-color: #2596be; border-radius: 5px; padding: 0px;\
                }\
                QPushButton#menu {\
                    color: white; background-color: #2596be; border-radius: 25px;\
                }\
                QPushButton:hover {\
                    background-color: #1d7595;\
                }\
                QPushButton#menu:hover {\
                    background-color: #1d7595;\
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
                    padding: 20px;\
                    border-radius: 10px;\
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
                    background-color: #1d7595;\
                    border-radius: 5px;\
                    height: 10px;\
                    width: 10px;\
                }\
                QGraphicsView {\
                    background-color: white;\
                    border-radius: 10px;\
                }\
                ";
    }else{//Dark mode
        this->set->setValue("set/darkmode",1);
        this->theme ="\
                QMainWindow {\
                    background-color: black;\
                }\
                QPushButton {\
                    color: white; background-color: #1d7595; border-radius: 5px;\
                }\
                QLabel#save {\
                    color: white; background-color: #1d7595; border-radius: 5px; padding: 0px;\
                }\
                QPushButton#menu {\
                    color: white; background-color: #1d7595; border-radius: 25px;\
                }\
                QPushButton:hover {\
                    background-color: #2596be;\
                }\
                QPushButton#menu:hover {\
                    background-color: #2596be;\
                }\
                QGroupBox {\
                    background-color: #1d7595;\
                    color: white;\
                    font-size: 15px;\
                    font-weight: bold;\
                    border-radius: 10px;\
                    padding: 20px\
                }\
                QLabel {\
                    background-color: #1d7595;\
                    color: white;\
                    padding: 20px;\
                    border-radius: 10px;\
                }\
                QSlider {\
                    background-color: #1d7595;\
                    color: white;\
                }\
                QSlider::groove:horizontal {\
                    height: 10px;\
                    border-radius: 5px;\
                    background-color: black;\
                    margin: 0px;\
                }\
                QSlider::handle:horizontal {\
                    background-color: #2596be;\
                    border-radius: 5px;\
                    height: 10px;\
                    width: 10px;\
                }\
                QGraphicsView {\
                    background-color: #303030;\
                    border-radius: 10px;\
                }\
                QChartView {\
                    background-color: #303030;\
                    border-radius: 10px;\
                }\
                ";

    }
    this->setStyleSheet(this->theme);
}

