#include "curve.h"

Curve::Curve(QCustomPlot *parent)
    : QCustomPlot(parent)
{
    setMinimumSize(400, 400);
    xAxis->setLabel(QString("Time"));
    yAxis->setLabel(QString("Value"));
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    xAxis->setTicker(timeTicker);
    xAxis->grid()->setVisible(false);
    yAxis->grid()->setVisible(false);
    yAxis->setScaleType(QCPAxis::stLinear);
    setNoAntialiasingOnDrag(true);
    legend->setVisible(true);
    legend->setBrush(QColor(255,255,255,0));
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
