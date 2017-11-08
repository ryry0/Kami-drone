#include "teensy_accel.h"
#include <Wire.h>


/**************************************************************************/
/*
 * INIT ACCEL
 */
/**************************************************************************/

void accel_init(void) {

  //set data format
  Wire.beginTransmission(ADXL345);
  Wire.write( DATA_FORMAT );
  Wire.write( 0x01 );
  Wire.endTransmission();

  //set set power to wakeup
  Wire.beginTransmission(ADXL345);
  Wire.write( POWER_CTL );
  Wire.write( 0x08 );
  Wire.endTransmission();

} //initAccel


/**************************************************************************/
/*
 * READ ACCEL
 */
/**************************************************************************/

void accel_read(volatile accel_data_t *accel_data) {
  uint8_t I2CSlaveBuffer[6] = {0};

  Wire.beginTransmission(ADXL345);
  Wire.write( ACCEL_DATA_X0 );
  Wire.endTransmission(); //i2cengine blocks until error or finished
  Wire.requestFrom(ADXL345, 6); //i2cengine blocks until error or finished

  size_t i = 0;
  while(Wire.available()) {
    I2CSlaveBuffer[i] = Wire.read();
    i++;
  }

  accel_data->raw_x = ((I2CSlaveBuffer[1] << 8) | I2CSlaveBuffer[0]) -
    accel_data->x_off;
  accel_data->raw_y = ((I2CSlaveBuffer[3] << 8) | I2CSlaveBuffer[2]) -
    accel_data->y_off;
  accel_data->raw_z = ((I2CSlaveBuffer[5] << 8) | I2CSlaveBuffer[4]) -
    accel_data->z_off;

} //readAccel


/**************************************************************************/
/*
 * CALIBRATE ACCEL
 */
/**************************************************************************/

void accel_calibrate(volatile accel_data_t *accel_data) {
  int16_t x_sum = 0, y_sum = 0, z_sum = 0;

  accel_data->x_off = 0;
  accel_data->y_off = 0;
  accel_data->z_off = 0;

  for (uint8_t i = 0; i < CALIB_SAMPLES; i++) {
    accel_read(accel_data);

    x_sum += accel_data->raw_x;
    y_sum += accel_data->raw_y;
    z_sum += accel_data->raw_z;
  }

  accel_data->x_off = x_sum/CALIB_SAMPLES;
  accel_data->y_off = y_sum/CALIB_SAMPLES;
  accel_data->z_off = (z_sum/CALIB_SAMPLES) - 256;
  //assuming 1g on z axis
} //end calibrateAccel

