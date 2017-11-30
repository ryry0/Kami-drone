#ifndef PACKET_DRONE_H_
#define PACKET_DRONE_H_
#include "packet.h"

typedef enum pkt_type_e {
  PKT_ACK         = 0x0A,
  PKT_DATA_START,
  PKT_LOG_PARAMS,
  PKT_COMMAND,
  PKT_SET_PARAMS,
  PKT_GET_PARAMS,
  PKT_SET_STATE,
  PKT_CALIBRATE,
  PKT_QUERY,
  PKT_KILL,
  PKT_TAKEOFF,
  PKT_SYN         = 0xFF
} pkt_type_t;


/** Macro to easily add parameters and ensure that they are synced with gui
 * labels */
#define FOREACH_PARAM(PARAM)            \
  PARAM(PKT_ROLL_KP)                   \
  PARAM(PKT_ROLL_KI)                   \
  PARAM(PKT_ROLL_KD)                   \
  PARAM(PKT_PITCH_KP)                   \
  PARAM(PKT_PITCH_KI)                   \
  PARAM(PKT_PITCH_KD)                   \
  PARAM(PKT_TAKEOFF_THROTTLE)       \
  PARAM(PKT_PARAM_MAX)              \

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

typedef enum pkt_param_e {
  FOREACH_PARAM(GENERATE_ENUM)
} pkt_param_t;

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
  float roll_commanded;
  float pitch_commanded;
  uint16_t throttle_commanded;
} pkt_command_t;

/*----------------------------------------------------------------------------*/
typedef struct {
  char msg[MAX_PAYLOAD_LENGTH];
} pkt_str_msg_t;


#endif
