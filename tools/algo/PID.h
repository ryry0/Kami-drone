/** \file
 * \author Ryan-David Reyes
 *
 * \brief This file contains several different implementations of PID control
 * schemes.
 */
#ifndef PID_H_
#define PID_H_
#include <ring_buffer.h>

/** \brief This macro sets the internal pid buffer size, for previous values.
 * */
#define PID_BUFFER_SIZE 6


/** \brief This data structure represents the PID, can create multiple PIDs per
 * motor */
typedef struct pid_data_t {
  float proportional_gain;
  float integral_gain;
  float derivative_gain;
  float previous_error;
  float integral_error;
  float max_integral_guard;
  float min_integral_guard;
  float pid_output;

  struct ring_buffer_s error_buffer_ring;
  struct ring_buffer_s output_buffer_ring;

  //private
  float error_buffer[PID_BUFFER_SIZE];
  float output_buffer[PID_BUFFER_SIZE];
} pid_data_t;

/** \brief Function pointer type for differnt PID methods */
typedef void (*pid_method_t)(pid_data_t *, float, float);


/**
 * \brief Correctly intialize and set the constants of the PID.
 *
 * \param pid The pid struct.
 * \param proportional_gain The proportional gain of the PID.
 * \param integral_gain The integral gain of the PID.
 * \param derivative_gain The derivative gain of the PID.
 * \param max_integral_guard Maximum that the running integral can sum to.
 * \param min_integral_guard Minimum that the running integral can sum to.
 */
void pid_init(pid_data_t *pid,
    float proportional_gain,
    float integral_gain,
    float derivative_gain,
    float max_integral_guard,
    float min_integral_guard);

/**
 * \brief Set the constants of the PID.
 *
 * \param pid The pid struct.
 * \param proportional_gain The proportional gain of the PID.
 * \param integral_gain The integral gain of the PID.
 * \param derivative_gain The derivative gain of the PID.
 * \param max_integral_guard Maximum that the running integral can sum to.
 * \param min_integral_guard Minimum that the running integral can sum to.
 */
void pid_setConstants(pid_data_t *pid,
    float proportional_gain,
    float integral_gain,
    float derivative_gain,
    float min_integral_guard,
    float max_integral_guard);

/**
 * \brief Performs a traditional PID calculation
 *
 * \param pid The pid struct.
 * \param current_error The error on current iteration.
 * \param dt Time since last update.
 */
void pid_update(pid_data_t *pid, float current_error, float dt);


/**
 * \brief Performs a traditional PID calculation meant to run at constant
 * intervals and does not need dt. It is more efficient than pid_update.
 *
 * dt is included in the function signature to match the others.
 *
 * \param pid The pid struct.
 * \param current_error The error on current iteration.
 * \param dt Time since last update.
 */
void pid_fixedUpdate(pid_data_t *pid, float current_error, float dt);

/**
 * \brief Performs a PID calculation in velocity form.
 *
 * The velocity from of the PID is where the derivative of the traditional PID
 * is taken, updated, then integrated to form the output. It has high
 * disturbance rejection due to smoothing using multi-point derivatives.
 *
 * Anti-windup for this method is easily implemented due to elimination of error
 * integral.
 *
 * Bumpless algorithm, can change parameters on teh fly without integral term
 * going wild.
 *
 * \param pid The pid struct.
 * \param current_error The error on current iteration.
 * \param dt Time since last update.
 */
void pid_velocUpdate(pid_data_t *pid, float current_error,
    float dt);

/**
 * \brief Minimal PI implementation that does not use derivative action.
 *
 * \param pid The pid struct.
 * \param current_error The error on current iteration.
 * \param dt Time since last update.
 */
void pid_minPIUpdate(pid_data_t *pid, float current_error,
    float dt);

/**
 * \brief Perform traditional feedback control by specifying a Pid controller
 * and method, the setpoint, the feedback sensor value, and time interval since
 * last update.
 *
 * \param pid The pid struct.
 * \param setpoint Target of control process.
 * \param sensor The sensor feedback of the plant.
 * \param dt Time since last update.
 * \param method The pid calculation method.
 * \return The control signal.
 */
float pid_FeedbackCtrl(pid_data_t *pid, float setpoint,
    float sensor, float dt, pid_method_t method);


/**
 * \brief Perform feedforward and feedback control by specifying a Pid
 * controller and method, the setpoint, the feedback sensor value, and time
 * interval since last update.
 *
 * This form tends to have higher disturbance
 * rejection than traditional feedback forms.
 *
 * \param pid The pid struct.
 * \param setpoint Target of control process.
 * \param sensor The sensor feedback of the plant.
 * \param dt Time since last update.
 * \param method The pid calculation method.
 * \return The control signal.
 */
float pid_feedforwardFeedbackCtrl(pid_data_t *pid, float setpoint,
    float sensor, float dt, pid_method_t method);

#endif
