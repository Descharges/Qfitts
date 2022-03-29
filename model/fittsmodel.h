#ifndef FITTSMODEL_H
#define FITTSMODEL_H

#include <QList>
#include <QPoint>

class FittsView;
class FittsController;

class FittsModel
{
public:
    FittsModel(FittsController *fittsController);
    void calculateResult();

private:
    FittsView *fittsView;

    int cibleLeft = 0;
    int nbCible = 10;
    int minSize = 20;
    int maxSize = 150;

    double a = 0.20;
    double b = 0.10;

    double ecartType = 0;
    double erreurType = 0;
    double diffMoy = 0;
    double itc95 = 0;

    QList<QPoint> clickPoints;
    QList<QPoint> cercleCenter;
    QList<int> cercleSize;
    QList<qint64> times;

    float ratio = 1;
    float failedClicks = 0;
    float clicks = 0;

    friend FittsView;
    friend FittsController;
};

#endif // FITTSMODEL_H
