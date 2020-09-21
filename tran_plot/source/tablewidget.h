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

    QStringList header1 = {"error_register"
                        , "modes_of_operation_display"
                        , "motor_q_axis_current_demand"
                        , "friction_estimate"
                        , "acceleration_based_on_link_encoder"
                        , "acceleration_based_on_motor_encoder"
                        , "tx_count"
                        , "statusword"
                        , "position_demand_value"
                        , "position_value_based_on_motor_encoder"
                        , "position_value_based_on_link_encoder"
                        , "speed_demand_of_motor_side"
                        , "speed_based_on_motor_encoder"
                        , "speed_based_on_link_encoder"
                        , "torque_actual_value_a"
                        , "torque_actual_value_b"
                        , "torque_actual_value_a_derivate"
                        , "torque_actual_value_b_derivate"
                        , "following_error_actual_value"
                        , "speed_control_integral_value"
                        , "digital_inputs"
                        , "current_position_actual_value"};
    QStringList header2 = {"joint1"
            , "joint2"
            , "joint3"
            , "joint4"
            , "joint5"
            , "joint6"
            , "joint7"};


public slots:
    void replot1();

private:

};

#endif // TABLEWIDGET_H
