#include "fittsmodel.h"
#include "fittsview.h"
#include <iostream>
#include <QDomDocument>
#include <QTextStream>
#include <QDebug>


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

void FittsModel::exportXML(){
    QString fileName = QFileDialog::getSaveFileName(NULL, "Export Test","export.xml","*.xml");

    QFile expFile(fileName);

    if(expFile.open(QFile::WriteOnly | QFile::Text)){

        QTextStream xmlContent(&expFile);

        QDomDocument doc;

        QDomElement root = doc.createElement("export");

        doc.appendChild(root);

        QDomElement buffer = doc.createElement("occurences");
        buffer.setAttribute("n",this->nbCible);
        root.appendChild(buffer);

        buffer = doc.createElement("missed");
        buffer.setAttribute("n",this->failedClicks);
        root.appendChild(buffer);

        buffer = doc.createElement("clicks");
        buffer.setAttribute("n",this->clicks);
        root.appendChild(buffer);

        QDomElement buffer2;

        buffer = doc.createElement("clickpoints");
        for(int i=0; i<this->nbCible; i++){
            buffer2 = doc.createElement("point");
            buffer2.setAttribute("x",this->clickPoints[i].x());
            buffer2.setAttribute("y",this->clickPoints[i].y());
            buffer.appendChild(buffer2);
        }
        root.appendChild(buffer);

        buffer = doc.createElement("circlecenter");
        for(int i=0; i<this->nbCible; i++){
            buffer2 = doc.createElement("point");
            buffer2.setAttribute("x",this->cercleCenter[i].x());
            buffer2.setAttribute("y",this->cercleCenter[i].y());
            buffer.appendChild(buffer2);
        }
        root.appendChild(buffer);

        buffer = doc.createElement("circlesize");
        for(int i=0; i<this->nbCible; i++){
            buffer2 = doc.createElement("size");
            buffer2.setAttribute("x",this->cercleSize[i]);
            buffer.appendChild(buffer2);
        }
        root.appendChild(buffer);

        buffer = doc.createElement("times");
        for(int i=0; i<this->nbCible; i++){
            buffer2 = doc.createElement("time");
            buffer2.setAttribute("x",this->times[i]);
            buffer.appendChild(buffer2);
        }
        root.appendChild(buffer);




        xmlContent << doc.toString();



        QMessageBox::information(NULL,"QFitts","File exported succesfully");
    }else{
         QMessageBox::information(NULL,"QFitts","Exportation failed :(");
    }

}

void FittsModel::importXML(){

    QString fileName = QFileDialog::getOpenFileName(NULL, "Importer un test","","*.xml");

    QFile impFile(fileName);

    if (impFile.open(QIODevice::ReadOnly )){

        qDebug() << "starting importation...";

        this->fittsView->graphicView->setEnabled(true);
        this->cercleSize.clear();
        this->cercleCenter.clear();
        this->clickPoints.clear();
        this->times.clear();


        QDomDocument doc;
        doc.setContent(&impFile);

        QDomElement root = doc.documentElement();

        QDomElement node = root.elementsByTagName("occurences").item(0).toElement();
        this->nbCible = node.attribute("n","0").toInt();
        qDebug() << this->nbCible << Qt::endl;

        node = root.elementsByTagName("missed").item(0).toElement();
        this->failedClicks = node.attribute("n","0").toInt();
        qDebug() << this->failedClicks<< "\n";

        node = root.elementsByTagName("clicks").item(0).toElement();
        this->clicks = node.attribute("n","0").toInt();
        qDebug() << this->clicks<< "\n";

        node = root.elementsByTagName("clickpoints").item(0).toElement();
        QDomElement buff = node.firstChildElement();

        for(int i=0; i<this->nbCible;i++){
            int x = buff.attribute("x","0").toInt();
            int y = buff.attribute("y","0").toInt();
            this->clickPoints.append(QPoint(x,y));
            qDebug() << this->clickPoints[i].x()<< "\n";
            qDebug() << this->clickPoints[i].y()<< "\n";
            buff = buff.nextSibling().toElement();
        }

        node = root.elementsByTagName("circlecenter").item(0).toElement();
        buff = node.firstChildElement();

        for(int i=0; i<this->nbCible;i++){
            int x = buff.attribute("x","0").toInt();
            int y = buff.attribute("y","0").toInt();
            this->cercleCenter.append(QPoint(x,y));
            qDebug() << this->cercleCenter[i].x()<< "\n";
            qDebug() << this->cercleCenter[i].y()<< "\n";
            buff = buff.nextSibling().toElement();
        }

        node = root.elementsByTagName("circlesize").item(0).toElement();
        buff = node.firstChildElement();

        for(int i=0; i<this->nbCible;i++){
            this->cercleSize.append(buff.attribute("x","0").toInt());
            qDebug() << this->cercleSize[i]<< "\n";
            buff = buff.nextSibling().toElement();
        }

        node = root.elementsByTagName("times").item(0).toElement();
        buff = node.firstChildElement();

        for(int i=0; i<this->nbCible;i++){
            this->times.append(buff.attribute("x","0").toInt());
            qDebug() << this->times[i]<< "\n";
            buff = buff.nextSibling().toElement();
        }

        this->calculateResult();

    }else{
       QMessageBox::information(NULL,"QFitts","Impossible d'ouvrir le fichier :(");
    }
}
