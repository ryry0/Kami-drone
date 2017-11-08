#include "teensy_gyro.h"
#include <Wire.h>


/**************************************************************************/
/*
 * INIT GYRO
 */
/**************************************************************************/
void initGyro(void) {
  //set pwr_mgm to zero
  Wire.beginTransmission(ITG3205);
  Wire.write(PWR_MGM);
  Wire.write(0x00);
  Wire.endTransmission();

  //set sample rate divider to 0x07
  Wire.beginTransmission( ITG3205);
  Wire.write( SMPLRT_DIV);
  Wire.write( 0x07);
  Wire.endTransmission();

  //set Digital lpf to 0x1e
  Wire.beginTransmission( ITG3205);
  Wire.write( DLPF_FS);
  Wire.write( 0x1E);
  Wire.endTransmission();

  //set interrupt config to 0x0
  Wire.beginTransmission( ITG3205);
  Wire.write( INT_CFG);
  Wire.write( 0x00);
  Wire.endTransmission();

} //end gyroInit

/**************************************************************************/
/*
 * CALIBRATE GYRO
 */
/**************************************************************************/
void calibrateGyro(volatile gyro_data_t *gyro_data) {
  int16_t roll_dot_sum = 0, pitch_dot_sum = 0, yaw_dot_sum = 0;

  gyro_data->roll_dot_off = 0;
  gyro_data->pitch_dot_off = 0;
  gyro_data->yaw_dot_off = 0;

  for (uint8_t i = 0; i < CALIB_SAMPLES; i++) {
    readGyro(gyro_data);

    roll_dot_sum += gyro_data->raw_roll_dot;
    pitch_dot_sum += gyro_data->raw_pitch_dot;
    yaw_dot_sum += gyro_data->raw_yaw_dot;
  }

  gyro_data->roll_dot_off = roll_dot_sum/CALIB_SAMPLES;
  gyro_data->pitch_dot_off = pitch_dot_sum/CALIB_SAMPLES;
  gyro_data->yaw_dot_off = yaw_dot_sum/CALIB_SAMPLES;
} //end calibrateGyro


/**************************************************************************/
/*
 * READ GYRO
 */
/**************************************************************************/
void readGyro(volatile gyro_data_t *gyro_data) {
  uint8_t I2CSlaveBuffer[6] = {0};
  Wire.beginTransmission( ITG3205);
  Wire.write( GYRO_XOUT_H);
  Wire.endTransmission(); //i2cengine blocks until error or finished

  Wire.requestFrom(ITG3205, 6); //i2cengine blocks until error or finished
  Wire.endTransmission(); //i2cengine blocks until error or finished

  size_t i = 0;
  while(Wire.available()) {
    I2CSlaveBuffer[i] = Wire.read();
    i++;
  }

  gyro_data->raw_roll_dot = ((I2CSlaveBuffer[0] << 8) | I2CSlaveBuffer[1]) -
    gyro_data->roll_dot_off;
  gyro_data->raw_pitch_dot = ((I2CSlaveBuffer[2] << 8) | I2CSlaveBuffer[3]) -
    gyro_data->pitch_dot_off;
  gyro_data->raw_yaw_dot = ((I2CSlaveBuffer[4] << 8) | I2CSlaveBuffer[5]) -
    gyro_data->yaw_dot_off;

} //end readGyro
