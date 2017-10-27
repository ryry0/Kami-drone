#ifndef PACKET_HYBRID_H_
#define PACKET_HYBRID_H_
#include <packet.h>

typedef enum pkt_type_e {
  PKT_ACK         = 0x0A,
  PKT_SET_PID,
  PKT_GET_PID,
  PKT_DBG_PID,
  PKT_ZERO_PID,
  PKT_STR_MSG,
  PKT_LOG_DATA,
  PKT_LOG_PARAMS,
  PKT_SET_PARAMS,
  PKT_GET_PARAMS,
  PKT_SET_STATE,
  PKT_CALIBRATE_KNEE,
  PKT_DATA_START  = 's',
  PKT_DATA_STOP   = 'n',
  PKT_EN_MOTOR    = 'e',
  PKT_DIS_MOTOR   = 'd',
  PKT_SYN         = 0xFF
} pkt_type_t;


/** Macro to easily add parameters and ensure that they are synced with gui
 * labels */
#define FOREACH_PARAM(PARAM)            \
  PARAM(MAX_HIP_ANGLE)              \
  PARAM(MIN_HIP_ANGLE)              \
  PARAM(MAX_KNEE_ANGLE)             \
  PARAM(L_KNEE_POSITION_PID_KP)     \
  PARAM(L_KNEE_POSITION_PID_KI)     \
  PARAM(L_KNEE_POSITION_PID_KD)     \
  PARAM(L_KNEE_POSITION_PID_AW_UB)  \
  PARAM(L_KNEE_POSITION_PID_AW_LB)  \
  PARAM(L_KNEE_SPEED_PID_KP)        \
  PARAM(L_KNEE_SPEED_PID_KI)        \
  PARAM(L_KNEE_SPEED_PID_KD)        \
  PARAM(L_KNEE_SPEED_PID_AW_UB)     \
  PARAM(L_KNEE_SPEED_PID_AW_LB)     \
  PARAM(L_KNEE_FC_ALPHA)            \
  PARAM(R_KNEE_POSITION_PID_KP)     \
  PARAM(R_KNEE_POSITION_PID_KI)     \
  PARAM(R_KNEE_POSITION_PID_KD)     \
  PARAM(R_KNEE_POSITION_PID_AW_UB)  \
  PARAM(R_KNEE_POSITION_PID_AW_LB)  \
  PARAM(R_KNEE_SPEED_PID_KP)        \
  PARAM(R_KNEE_SPEED_PID_KI)        \
  PARAM(R_KNEE_SPEED_PID_KD)        \
  PARAM(R_KNEE_SPEED_PID_AW_UB)     \
  PARAM(R_KNEE_SPEED_PID_AW_LB)     \
  PARAM(R_KNEE_FC_ALPHA)            \
  PARAM(WEIGHT_ACC_FSR_THRESH)      \
  PARAM(LOW_FSR_THRESH)             \
  PARAM(HIGH_FSR_THRESH)            \
  PARAM(POSITION_CONTROL)           \
  PARAM(AUTO_WALK)                  \
  PARAM(TF)                         \
  PARAM(TPS_PERC)                   \
  PARAM(L_KNEE_BC_THRESH)           \
  PARAM(L_KNEE_BC_CURRENT)          \
  PARAM(R_KNEE_BC_THRESH)           \
  PARAM(R_KNEE_BC_CURRENT)          \
  PARAM(MOTOR_FORCE_OFF)          \
  PARAM(PKT_PARAM_MAX)              \

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

typedef enum pkt_param_e {
  FOREACH_PARAM(GENERATE_ENUM)
} pkt_param_t;

typedef enum pkt_pid_id_e {
  PKT_PID_L_KNEE_POSITION,
  PKT_PID_L_KNEE_SPEED,

  PKT_PID_R_KNEE_POSITION,
  PKT_PID_R_KNEE_SPEED,
} pkt_pid_id_t;
/*----------------------------------------------------------------------------*/
typedef struct {
  float setpoint;
  float Kp;
  float Ki;
  float Kd;
  float current;
  float speed;
  float position;
  float current_command;
  float speed_command;
  float pwm_command;
  float direction;
  uint8_t pid_id;
} pkt_dbg_pid_t;

/*----------------------------------------------------------------------------*/
typedef struct {
  float r_hip_angle;
  float l_hip_angle;

  float r_knee_current;
  float r_knee_velocity;
  float r_knee_angle;

  float r_knee_current_commanded;
  float r_knee_velocity_commanded;
  float r_knee_angle_commanded;

  float l_knee_current;
  float l_knee_velocity;
  float l_knee_angle;

  float l_knee_current_commanded;
  float l_knee_velocity_commanded;
  float l_knee_angle_commanded;

  uint32_t timestamp;

  uint16_t an_0_l_fsr_heel;
  uint16_t an_2_r_fsr_heel;
  uint16_t an_1_l_fsr_ff;
  uint16_t an_3_r_fsr_ff;
  uint16_t an_11_bat_volt;

  uint8_t state;
  uint8_t time_marker;
} pkt_log_data_t;

/*----------------------------------------------------------------------------*/
typedef struct {
  //just send a bunch of floats they can get typecasted in the firmware.
  //You'll only get a couple bytes of overhead anyway.
  //It is too painful to need to update struct names manually every time.
  float float_params[PKT_PARAM_MAX];
} pkt_log_params_t;

typedef pkt_log_params_t pkt_set_params_t;
typedef pkt_log_params_t pkt_get_params_t;

/*----------------------------------------------------------------------------*/
typedef struct {
  uint8_t state;
} pkt_set_state_t;

/*----------------------------------------------------------------------------*/
typedef struct {
  float setpoint;
  float Kp;
  float Ki;
  float Kd;
  float guard_max;
  float guard_min;
  uint8_t pid_id;
} pkt_set_pid_t;

typedef pkt_set_pid_t pkt_get_pid_t;

/*----------------------------------------------------------------------------*/
typedef struct {
  char msg[MAX_PAYLOAD_LENGTH];
} pkt_str_msg_t;


#endif
