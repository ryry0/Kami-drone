/** \file
 * \author Ryan-David Reyes
 *
 * \brief This file contains useful numerical methods.
 */

#ifndef NUMERICAL_H_
#define NUMERICAL_H_
#include <ring_buffer.h>

/**
 * \brief This macro forces an amount to be constrained between low and high
 * values.
 *
 * \param amt The amount.
 * \param low The lower bound.
 * \param high The upper bound.
 */
#define nm_constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

/**
 * \brief This function takes a 2 point or 1st order approximation of the first
 * derivative.
 *
 * \param r_buff Ring buffer of size 2
 * \param dt Sample time between the points.
 */
float nm_fdFirstDer2(const ring_buffer_t r_buff, float dt);

/**
 * \brief This function takes a 5 point or 5th order approximation of the first
 * derivative.
 *
 * \param r_buff Ring buffer of size 6
 * \param dt Sample time between the points.
 */
float nm_fdFirstDer5(const ring_buffer_t r_buff, float dt);

/**
 * \brief This function takes a 3 point or 1st order finite difference
 * approximation of the second derivative.
 *
 * \param r_buff Ring buffer of size 3
 * \param dt Sample time between the points.
 */
float nm_fdSecondDer3(const ring_buffer_t r_buff, float dt);

/**
 * \brief This function takes a 5 point or 4th order finite difference
 * approximation of the second derivative.
 *
 * \param r_buff Ring buffer of size 6
 * \param dt Sample time between the points.
 */
float nm_fdSecondDer5(const ring_buffer_t r_buff, float dt);

/**
 * \brief This function takes a window size 5 savitsky golay approximation of
 * the second derivative.
 *
 * \param r_buff Ring buffer of size 6
 * \param dt Sample time between the points.
 */
float nm_sgSecondDer(const ring_buffer_t r_buff, float dt);

/**
 * \brief This function takes a 6th order savitsky golay approximation of the
 * second derivative.
 *
 * \param r_buff Ring buffer of size 7
 * \param dt Sample time between the points.
 */
float nm_sgSecondDer6(const ring_buffer_t r_buff, float dt);


/**
 * \brief This function performs one iteration of a 2 point discrete integration.
 *
 * \param r_buff Ring buffer of size 4
 * \param dt Sample time between the points.
 */
float nm_dIntegrate2(const ring_buffer_t prev_vals, float du, float dt);


/**
 * \brief This function performs one iteration of a 4 point discrete integration.
 *
 * \param r_buff Ring buffer of size 4
 * \param dt Sample time between the points.
 */
float nm_dIntegrate4(const ring_buffer_t prev_vals, float du, float dt);

/**
 * \brief This function mallocs an array of floats between start and end, in
 * increments of increment.
 *
 * \param start First element of the array value
 * \param increment Space between values
 * \param end End value of the array
 * \param array Pointer which will hold the array when done.
 *
 * \return Returns the size of the array.
 */
size_t nm_genRange(float start, float increment, float end, float **array);

/**
 * \brief This function simulates a recursive first order exponential process:
 * 1-e^(-t/tau)
 *
 * \param prev Previous value of the process
 * \param setpoint Where the process is heading to
 * \param sample_time Time between the samples
 * \param tau Time constant of the exponential process. Should be roughly
 * .25*rise time of simulated system.
 *
 * \return Returns the current value of the process
 */
float nm_expProcess(float prev, float setpoint, float sample_time, float tau);


/**
 * \brief This function can perform a computationally efficient stateless
 * exponential moving average stored in current_average.
 *
 * \param constant Value for the constant used in the moving average. Must be (0,1)
 * The closer to zero, the lower the cutoff frequency.
 * \param current_average The current value for the moving average
 * \param new_value The new value to incorporate into the moving average
 *
 * \return Returns the updated value of the moving average
 */
float nm_expMovAvg(float constant, float current_average,
    float new_value);


/**
 * \brief This function can remove the dc offset in a running signal.
 *
 * Constant of 0.95 is typical.
 *
 * \param constant Value for the constant used in the filter. Must be (0,1)
 * \param y_prev The previous output value.
 * \param x The new value from the process.
 * \param x_prev The previous value from the process.
 *
 * \return Returns the updated value of the filter
 */
inline float nm_removeDC(float constant, float y_prev, float x, float x_prev) {
  return x - x_prev + constant*y_prev;
}
#endif
