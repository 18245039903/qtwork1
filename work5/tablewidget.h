#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QTableWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QTime>
#include <QString>
#include <math.h>

class TableWidget : public QTableWidget
{
    Q_OBJECT

public:
    TableWidget(QTableWidget *parent = nullptr);
    ~TableWidget();

public slots:
    void replot1();

private:
};

#endif // TABLEWIDGET_H
