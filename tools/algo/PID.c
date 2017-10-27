#include <PID.h>
#include <numerical.h>

void pid_init(pid_data_t *pid, float proportional_gain,
    float integral_gain, float derivative_gain,
    float min_integral_guard,
    float max_integral_guard) {
  pid_setConstants(
      pid,
      proportional_gain,
      integral_gain,
      derivative_gain,
      min_integral_guard,
      max_integral_guard);

  //set up the ring buffers
  rb_setBuffer(&pid->error_buffer_ring, pid->error_buffer);
  rb_setBuffer(&pid->output_buffer_ring, pid->output_buffer);

  //set contents of ring buffers to zero
  rb_init(&pid->error_buffer_ring, PID_BUFFER_SIZE);
  rb_init(&pid->output_buffer_ring, PID_BUFFER_SIZE);
  pid->pid_output = 0;
}

void pid_setConstants(pid_data_t *pid, float proportional_gain,
    float integral_gain, float derivative_gain,
    float min_integral_guard, float max_integral_guard) {
  pid->proportional_gain = proportional_gain;
  pid->integral_gain = integral_gain;
  pid->derivative_gain = derivative_gain;
  pid->max_integral_guard = max_integral_guard;
  pid->min_integral_guard = min_integral_guard;
  pid->previous_error = 0;
  pid->integral_error = 0;
}

void pid_update(pid_data_t *pid, float current_error, float dt) {
  float error_differential = 0;

  pid->integral_error += current_error * dt;
  pid->integral_error = nm_constrain(pid->integral_error,
      pid->min_integral_guard,
      pid->max_integral_guard);
  error_differential = (current_error - pid->previous_error)/dt;

  pid->pid_output = (pid->proportional_gain * current_error) +
    (pid->integral_gain    * pid->integral_error) +
    (pid->derivative_gain  * error_differential);

  pid->previous_error = current_error;
} //end pidControl()

//fixed update PID is meant to be called at constant time intervals,
//therefore it does not need dt
//It only has dt to match the function signature of the others
void pid_fixedUpdate(pid_data_t *pid, float current_error, float dt) {
  float error_differential = 0;

  pid->integral_error += current_error;
  pid->integral_error = nm_constrain(pid->integral_error, pid->min_integral_guard,
      pid->max_integral_guard);

  error_differential = (current_error - pid->previous_error);

  pid->pid_output = (pid->proportional_gain * (current_error)) +
    (pid->integral_gain    * pid->integral_error) +
    (pid->derivative_gain  * error_differential);

  pid->previous_error = current_error;
} //end pidControl()

void pid_velocUpdate(pid_data_t *pid, float current_error,
    float dt) {
  const float Kp = pid->proportional_gain;
  const float Ki = pid->integral_gain;
  const float Kd = pid->derivative_gain;
  struct ring_buffer_s *const error_buffer = &pid->error_buffer_ring;
  struct ring_buffer_s *const output_buffer = &pid->output_buffer_ring;

  rb_pushFront(error_buffer, current_error); //update the error buffer

  const float du =
    Kp * nm_fdFirstDer2(error_buffer, dt) +
    Ki * current_error +
    Kd * nm_fdSecondDer3(error_buffer, dt);

  float u = nm_dIntegrate2(output_buffer, du, dt);

  //printf("gl %f gh %f u before constrain: %f ", pid->min_integral_guard, pid->max_integral_guard, u);

  // antiwindup
  u = nm_constrain(u, pid->min_integral_guard, pid->max_integral_guard);

  //printf("u after constrain: %f\n", u);

  rb_pushFront(output_buffer, u); //update the error buffer
  pid->pid_output = u;
}

void pid_minPIUpdate(pid_data_t *pid, float current_error,
    float dt) {
  const float Kp = pid->proportional_gain;
  const float Ki = pid->integral_gain;
  struct ring_buffer_s *const error_buffer = &pid->error_buffer_ring;
  struct ring_buffer_s *const output_buffer = &pid->output_buffer_ring;

  rb_pushFront(error_buffer, current_error); //update the error buffer

  float u =
    rb_get(output_buffer, 0) + //u[n-1]
    -Kp*rb_get(error_buffer, 1) + //Kp*e[n-1]
    (Ki*dt + Kp)*rb_get(error_buffer, 0); //(Ki*dt - Kp) * e[n]

  // antiwindup
  u = nm_constrain(u, pid->min_integral_guard, pid->max_integral_guard);

  rb_pushFront(output_buffer, u); //update the output buffer
  pid->pid_output = u;
}

float pid_FeedbackCtrl(pid_data_t *pid, float setpoint,
    float sensor, float dt, pid_method_t method) {
  //get error
  const float error = setpoint - sensor;
  //update PID
  (*method)(pid, error, dt);

  return pid->pid_output;
}

float pid_feedforwardFeedbackCtrl(pid_data_t *pid, float setpoint,
    float sensor, float dt, pid_method_t method) {

  return setpoint - pid_FeedbackCtrl(pid, setpoint, sensor, dt, method);
}
