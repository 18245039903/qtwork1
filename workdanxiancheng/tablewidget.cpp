#include "tablewidget.h"
#include "widget.h"

TableWidget::TableWidget(QTableWidget *parent)
    : QTableWidget(parent)
{
    setMinimumSize(600, 600);
    setColumnCount(7);
    setRowCount(22);
    QStringList header;
    header << "error_register"
           << "modes_of_operation_display"
           << "motor_q_axis_current_demand"
           << "friction_estimate"
           << "acceleration_based_on_link_encoder"
           << "acceleration_based_on_motor_encoder"
           << "tx_count"
           << "statusword"
           << "position_demand_value"
           << "position_value_based_on_motor_encoder"
           << "position_value_based_on_link_encoder"
           << "speed_demand_of_motor_side"
           << "speed_based_on_motor_encoder"
           << "speed_based_on_link_encoder"
           << "torque_actual_value_a"
           << "torque_actual_value_b"
           << "torque_actual_value_a_derivate"
           << "torque_actual_value_b_derivate"
           << "following_error_actual_value"
           << "speed_control_integral_value"
           << "digital_inputs"
           << "current_position_actual_value";
    setVerticalHeaderLabels(header);
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
