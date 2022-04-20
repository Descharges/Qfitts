#include "fittsmodel.h"
#include "fittsview.h"


FittsModel::FittsModel(FittsController *fittsController) {
    this->fittsView = new FittsView(this, fittsController);

}

void FittsModel::calculateResult()
{
    //Premier graphique

    QChart *chart = new QChart;
    this->fittsView->plot->setChart(chart);
    this->fittsView->plot->setRenderHint(QPainter::Antialiasing);

    chart->setTitle("Résultats loi Fitts (temps/distance en px)");
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->createDefaultAxes();
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QLineSeries *expSeries = new QLineSeries;
    expSeries->setName("Courbe expérimentale");
    QLineSeries *fittsSeries = new QLineSeries;
    fittsSeries->setName("Courbe théorique");
    QCategoryAxis *axis = new QCategoryAxis;

    QList<double> fittsValues;

    for(int i = 0; i < this->nbCible; ++i) {

        double D = sqrt(pow(this->clickPoints[i].x() - this->cercleCenter[i].x(),2) + pow(this->clickPoints[i].y() - this->cercleCenter[i].y(),2));
        double Fitts = (this->a * 1000) + ((this->b * 1000) * log2((D / this->cercleSize[i]) + 1)); //*1000 pour avoir le résultat en ms
        double T = this->times[i];
        expSeries->append(i,T);

        fittsValues.append(Fitts);
        fittsSeries->append(i,Fitts);

        axis->append("<br />D" + QString::number(i) + " : " + QString::number(round(D)),i);
    }
    axis->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
    axis->setLabelsAngle(80);

    chart->addSeries(expSeries);
    chart->addSeries(fittsSeries);

    chart->setAxisX(axis,expSeries);
    chart->setAxisX(axis,fittsSeries);



    QValueAxis *axisY = new QValueAxis;
    axisY->setTitleText("temps (en ms)");
    chart->setAxisY(axisY,expSeries);


    //Second graphique

    QChart *chart2 = new QChart;
    this->fittsView->plot2->setChart(chart2);
    this->fittsView->plot2->setRenderHint(QPainter::Antialiasing);

    chart2->setTitle("Résultats loi de Fitts (temps/log(2*distance/diametre))");
    chart2->setAnimationOptions(QChart::AllAnimations);
    chart2->createDefaultAxes();
    chart2->legend()->setVisible(true);
    chart2->legend()->setAlignment(Qt::AlignBottom);

    QLineSeries *expSeries2 = new QLineSeries;
    expSeries2->setName("Courbe expérimentale");
    QLineSeries *fittsSeries2 = new QLineSeries;
    fittsSeries2->setName("Courbe théorique");
    QCategoryAxis *axis2 = new QCategoryAxis;

    QVector<QPointF> expLine, fittsLine;

   for(int i = 0; i < nbCible; ++i) {

        double D = sqrt(pow(this->clickPoints[i].x() - this->cercleCenter[i].x(),2) + pow(this->clickPoints[i].y() - this->cercleCenter[i].y(),2));
        double M = log2(2*D/this->cercleSize[i]);
        double T = this->times[i];
        double Fitts = (this->a * 1000) + ((this->b * 1000) * log2((D / this->cercleSize[i]) + 1)); //*1000 pour avoir le résultat en ms

        QPointF expPts, fittsPts;

        expPts.setX(M);
        expPts.setY(T);
        fittsPts.setX(M);
        fittsPts.setY(Fitts);

        expLine.append(expPts);
        fittsLine.append(fittsPts);
    }
    std::sort(expLine.begin(), expLine.end(), [](const QPointF & p1, const QPointF & p2){
        return p1.x() < p2.x();
    }); //Cette fonction lambda est une fonction de tri d'un vecteur de point supporté par std trouvé sur internet
    expSeries2->replace(expLine);

    std::sort(fittsLine.begin(), fittsLine.end(), [](const QPointF & p1, const QPointF & p2){
        return p1.x() < p2.x();
    }); //Cette fonction lambda est une fonction de tri d'un vecteur de point supporté par std trouvé sur internet
    fittsSeries2->replace(fittsLine);

    for(int i = 0; i < nbCible; i++){
        axis2->append(QString::number(expLine[i].x()),expLine[i].x());
    }

    axis2->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
    axis2->setLabelsAngle(80);

    chart2->addSeries(expSeries2);
    chart2->addSeries(fittsSeries2);

    chart2->setAxisX(axis2,expSeries2);
    chart2->setAxisX(axis2,fittsSeries2);

    QValueAxis *axisY2 = new QValueAxis;
    axisY2->setTitleText("temps (en ms)");
    chart2->setAxisY(axisY2,expSeries2);



    // Calcul des valeurs
    // Moyennes
    QList<double> diffValues;
    double diffMoy = 0;

    for (int i = 0; i < fittsValues.size(); ++i) {
        diffValues.append(fabs(fittsValues[i] - this->times[i]));
        diffMoy += fabs(fittsValues[i] - this->times[i]);
    }
    diffMoy /= fittsValues.size();

    // On stock la difference de moyenne
    this->diffMoy = fabs(diffMoy);


    // Ecart type
    double variance = 0;

    for (int i = 0; i < fittsValues.size(); ++i) {
        variance += pow(diffValues[i] - diffMoy,2);
    }
    variance /= fittsValues.size();

    double ecartType = sqrt(variance);

    // On stock l'ecart type
    this->ecartType = ecartType;
    // On stock l'erreur type
    this->erreurType = fabs(ecartType / sqrt(fittsValues.size()));

    // On stock itc 95%
    this->itc95 = 2 * this->erreurType;


    //ratio
    float fail = failedClicks;
    float succes = clicks;
    float total = fail + succes;
    ratio = succes / total;

    this->fittsView->displayResults();
}
