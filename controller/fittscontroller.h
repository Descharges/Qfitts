#ifndef FITTSCONTROLLER_H
#define FITTSCONTROLLER_H

#include <QObject>
#include <QElapsedTimer>

class FittsView;
class FittsModel;

//Define the result of an interaction with a widget
class FittsController : public QObject
{
    Q_OBJECT
public:
    FittsController();
    virtual ~FittsController() {}
    void start();

private:
    void initGame();
    void finish();
    void nextCible();

    void calculateResult();

    FittsModel *fittsModel;
    FittsView *fittsView;

    QElapsedTimer *timer;

private slots:
    void quit();
    void startSimulation();
    void resultClicked();

    void toGraph1();
    void toGraph2();
    void toSettings();
    void toStats();

    void aValueChanged(int value);
    void bValueChanged(int value);
    void nbCibleChanged(int value);
    void minSizeChanged(int value);
    void maxSizeChanged(int value);

    void cibleClicked(int x, int y);

    friend FittsView;
};

#endif // FITTSCONTROLLER_H
