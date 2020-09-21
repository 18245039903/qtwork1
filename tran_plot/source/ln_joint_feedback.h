#ifndef LN_JOINT_FEEDBACK_H
#define LN_JOINT_FEEDBACK_H
#include <stdint.h>

#ifdef __cplusplus
#endif
/* topic/joint_feedback */
#pragma pack(1)
struct topic_joint_feedback_struct;
typedef struct topic_joint_feedback_struct topic_joint_feedback_t;
struct topic_joint_feedback_struct {
    uint8_t error_register[7];
    int8_t modes_of_operation_display[7];
    float motor_q_axis_current_demand[7];
    float friction_estimate[7];
    float acceleration_based_on_link_encoder[7];
    float acceleration_based_on_motor_encoder[7];
    uint16_t tx_count[7];
    uint16_t statusword[7];
    double position_demand_value[7];
    double position_value_based_on_motor_encoder[7];
    double position_value_based_on_link_encoder[7];
    float speed_demand_of_motor_side[7];
    float speed_based_on_motor_encoder[7];
    float speed_based_on_link_encoder[7];
    double torque_actual_value_a[7];
    double torque_actual_value_b[7];
    double torque_actual_value_a_derivate[7];
    double torque_actual_value_b_derivate[7];
    double following_error_actual_value[7];
    float speed_control_integral_value[7];
    uint16_t digital_inputs[7];
    double current_position_actual_value[7];
};
#pragma pack()
#endif // LN_JOINT_FEEDBACK_H
