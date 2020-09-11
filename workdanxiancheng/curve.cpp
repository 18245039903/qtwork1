#include "curve.h"

Curve::Curve(QCustomPlot *parent)
    : QCustomPlot(parent)
{
    setMinimumSize(600, 500);
    xAxis->setLabel(QString("Time"));
    yAxis->setLabel(QString("Value"));
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    xAxis->setTicker(timeTicker);
    xAxis->grid()->setVisible(false);
    yAxis->grid()->setVisible(false);
    setNoAntialiasingOnDrag(true);
}

Curve::~Curve()
{
    if(graph())
    {
       clearGraphs();
    }
}

void Curve::replot1()
{
    xAxis->setRange(0, 1.5, Qt::AlignLeft);
    clearGraphs();
    replot();
}

void Curve::replot2()
{
    replot(QCustomPlot::rpQueuedReplot);
}
