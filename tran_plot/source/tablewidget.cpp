#include "tablewidget.h"
#include "widget.h"

TableWidget::TableWidget(QTableWidget *parent)
    : QTableWidget(parent)
{
    setMinimumSize(400, 400);
    setColumnCount(7);
    setRowCount(22);
    setVerticalHeaderLabels(header1);
    setHorizontalHeaderLabels(header2);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionMode(QAbstractItemView::NoSelection);
}

TableWidget::~TableWidget()
{
}

void TableWidget::replot1()
{
    clearContents();
    show();
}
