#ifndef CURVE_H
#define CURVE_H

#include "qcustomplot.h"

class Curve : public QCustomPlot
{
    Q_OBJECT

public:
    Curve(QCustomPlot *parent = nullptr);
    ~Curve();

public slots:
    void replot1();
    void replot2();
    //void restore();
};

#endif // CURVE_H
