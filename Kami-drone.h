#ifndef KAMI_DRONE_H_
#include "teensy_accel.h"
#include "teensy_gyro.h"
#include "packet.h"

typedef enum kami_drone_state_e {
  STATE_LANDED,
  STATE_FLYING,
  STATE_TAKEOFF,
  STATE_HARD_KILL,
} kami_drone_state_t;

#endif KAMI_DRONE_H_
