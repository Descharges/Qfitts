#include "fittsview.h"
#include "graphicwidget.h"
#include "fittscontroller.h"
#include "fittsmodel.h"

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
    connect(startBtn,SIGNAL(clicked()),fittsController,SLOT(startSimulation()));

    connect(resultBtn,SIGNAL(clicked()),fittsController,SLOT(resultClicked()));

    connect(resultLeaveBtn,SIGNAL(clicked()),fittsController,SLOT(quit()));

    connect(graphicView, SIGNAL(mouseClicked(int,int)), fittsController, SLOT(cibleClicked(int,int)));
    connect(graph1, SIGNAL(clicked()), fittsController, SLOT(toGraph1()));
    connect(graph2, SIGNAL(clicked()), fittsController, SLOT(toGraph2()));

    connect(settingBtn, SIGNAL(clicked()), fittsController, SLOT(toSettings()));
    connect(statBtn, SIGNAL(clicked()), fittsController, SLOT(toStats()));

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
    this->setStyleSheet("background-color: lightblue;");

    QLabel *label;


    QVBoxLayout *titleBtnLayout = new QVBoxLayout;


    settingBtn = new QPushButton("");
    settingBtn->setFixedSize(50,50);
    settingBtn->setStyleSheet("color: white; background-color: darkslateblue; border-radius: 25px");
    settingBtn->setIcon(QIcon("/home/paulc/Projets/Qfitts/media/set.png"));
    settingBtn->setIconSize(QSize(30, 30));
    titleBtnLayout->addWidget(settingBtn);

    statBtn = new QPushButton("");
    statBtn->setFixedSize(50,50);
    statBtn->setStyleSheet("color: white; background-color: darkslateblue; border-radius: 25px");
    statBtn->setIcon(QIcon("/home/paulc/Projets/Qfitts/media/stat.png"));
    statBtn->setIconSize(QSize(40, 40));
    titleBtnLayout->addWidget(statBtn);

    titleBtnLayout->addStretch();

    QVBoxLayout *titles = new QVBoxLayout;

    titles->addLayout(titleBtnLayout);




    QWidget *mainWidget = new QWidget;
    this->setCentralWidget(mainWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(mainWidget);

    mainLayout->addLayout(titles);

    mainStack = new QStackedLayout;
    mainLayout->addLayout(mainStack);




    //setting

    QWidget *settingsWidget = new QWidget;
    mainStack->addWidget(settingsWidget);

    QVBoxLayout *settingsLayout = new QVBoxLayout(settingsWidget);


    QGroupBox *rappelBox = new QGroupBox("Formule de la loi de Fitts :");
    settingsLayout->addWidget(rappelBox);
    QHBoxLayout *rappelLayout = new QHBoxLayout(rappelBox);

    QVBoxLayout *rappelLeftLayout = new QVBoxLayout();
    rappelLayout->addLayout(rappelLeftLayout,2);

    label = new QLabel;
    label->setPixmap(QPixmap("C:/Users/Martin/Desktop/HM40 TP/support_TP_Fitts/fittsTP/image/fitts.png").scaled(500,400,Qt::KeepAspectRatio));
    rappelLeftLayout->addWidget(label);

    QGridLayout *rappelRightLayout = new QGridLayout();
    rappelLayout->addLayout(rappelRightLayout,1);

    QGroupBox *configBox = new QGroupBox("Configuration du test :");
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

    startBtn = new QPushButton("Démarrer");
    btnLayout->addWidget(startBtn);



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

    label = new QLabel("Cliquez sur les cibles qui apparaissent en <font color='red'><strong>rouge</strong></font>");
    testLayout->addWidget(label);

    btnLayout = new QHBoxLayout;
    testLayout->addLayout(btnLayout);

    resultBtn = new QPushButton("Résultats");
    btnLayout->addWidget(resultBtn);
    resultBtn->setEnabled(false);



    //graphic

    QWidget *resultWidget = new QWidget;

    mainStack->addWidget(resultWidget);

    QVBoxLayout *resultLayout = new QVBoxLayout(resultWidget);

    QHBoxLayout *statBtnLayout = new QHBoxLayout;

    graphStack = new QStackedLayout;
    resultLayout->addLayout(graphStack);


    graph1 = new QPushButton("T = f(D)");
    statBtnLayout->addWidget(graph1);

    graph2 = new QPushButton("T = log(2D/L)");
    statBtnLayout->addWidget(graph2);

    resultLayout->addLayout(statBtnLayout);

    plot = new QChartView;
    graphStack->addWidget(plot);

    plot2 = new QChartView;
    graphStack->addWidget(plot2);

    QGroupBox *resultBox =  new QGroupBox("Statistiques");
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

    label = new QLabel("Différence moyenne : ");
    resultBoxLayout->addWidget(label,0,2);
    diffMoy = new QLabel;
    resultBoxLayout->addWidget(diffMoy,0,3);

    label = new QLabel("Intervalle de confiance à 95% : ");
    resultBoxLayout->addWidget(label,1,2);
    itc95 = new QLabel;
    resultBoxLayout->addWidget(itc95,1,3);

    label = new QLabel("Cliques ratés : ");
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

    resultLeaveBtn = new QPushButton("Quitter");
    btnLayout->addWidget(resultLeaveBtn);

}

void FittsView::updateTestMsg() {
    this->testLabel->setText("<strong>Le test commencera après avoir appuyé sur la cible <font color='blue'><strong>bleu</strong></font>.</strong> Nombre de cibles restantes : " + QString::number(this->fittsModel->cibleLeft));
}


void FittsView::displayResults() {
    diffMoy->setText(QString::number(fittsModel->diffMoy));
    ecartType->setText(QString::number(fittsModel->ecartType));
    erreurType->setText(QString::number(fittsModel->erreurType));
    itc95->setText(QString::number(fittsModel->itc95));
    failedClicks->setText(QString::number(fittsModel->failedClicks));
    ratio->setText(QString::number(fittsModel->ratio));
}

