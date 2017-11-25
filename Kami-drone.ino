#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ring_buffer.h"
#include "teensy_accel.h"
#include "teensy_gyro.h"
#include "teensy_esp8266.h"
#include "numerical.h"
#include "packet.h"

#define OLED_RESET 15
#define I2C_SDA 34
#define I2C_SCL 33

#define MOTOR_PIN_1 36
#define MOTOR_PIN_2 37
#define MOTOR_PIN_3 38
#define MOTOR_PIN_4 35
#define PWM_FREQ 12000
#define MOTOR_ZERO_SPEED 20

#define TEST_PIN1 35
#define TEST_PIN2 36

#define FIL_ALPHA 0.995
#define DELTA_TIME 0.002
#define LOOP_CLOSURE_US 2000

#define I2C_FASTMODE 400000
#define WIFI_SERIAL Serial1
#define USB_SERIAL  Serial
#define WIFI_DEBUG

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

IntervalTimer control_timer;
Adafruit_SSD1306 display(OLED_RESET);


/**
 todo:
  Make wifi work
    recieve data
    activate on :\x00
    interpret data

  Get general offset

  Write control algo
    tune it
    tune cascaded

  Params
    PID params
    Calibrate

  Controller?
**/

typedef struct kami_drone_s {
  accel_data_t accel_data;
  gyro_data_t gyro_data;
  float roll;
  float pitch;
} kami_drone_t;

volatile kami_drone_t kami_drone;

void setupSerial() {
  USB_SERIAL.begin(115200);
  WIFI_SERIAL.begin(115200);
}

void setupPWM() {
  pinMode(MOTOR_PIN_1, OUTPUT);
  pinMode(MOTOR_PIN_2, OUTPUT);
  pinMode(MOTOR_PIN_3, OUTPUT);
  pinMode(MOTOR_PIN_4, OUTPUT);
  analogWriteFrequency(MOTOR_PIN_1, PWM_FREQ);
}

void setupWire() {
  Wire.setSDA(I2C_SDA);
  Wire.setSCL(I2C_SCL);
  Wire.begin();
  Wire.setClock(I2C_FASTMODE);
}

void setupDisplay() {
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
}

void setupWifi() {
  wifi_sendCommand(&WIFI_SERIAL, "ATE0\r\n");
}

void setup() {
  //-----------------
  setupSerial();
  setupWire();
  setupPWM();
  //setupDisplay();

  // init done
  //-----------------
  accel_init();
  gyro_init();
  accel_calibrate(&kami_drone.accel_data);
  gyro_calibrate(&kami_drone.gyro_data);
  control_timer.begin(controlLoop, LOOP_CLOSURE_US);
}

void controlLoop() {
  static float roll = 0;
  digitalWriteFast(TEST_PIN1, HIGH);
  digitalWriteFast(TEST_PIN2, HIGH);
  digitalWriteFast(TEST_PIN1, LOW);

  accel_convertToGs(&kami_drone.accel_data);
  accel_calcAngle(&kami_drone.accel_data);

  kami_drone.roll = nm_expMovAvg(FIL_ALPHA,
    kami_drone.accel_data.roll,
    kami_drone.roll + ((kami_drone.gyro_data.raw_roll_dot/GYRO_SCALING)*DELTA_TIME));

  kami_drone.pitch = nm_expMovAvg(FIL_ALPHA,
    kami_drone.accel_data.pitch,
    kami_drone.pitch + ((kami_drone.gyro_data.raw_pitch_dot/GYRO_SCALING)*DELTA_TIME));

  digitalWriteFast(TEST_PIN2, LOW);
}

void loop() {
  static int i = 0;
  static bool print_stuff = false;
  static bool motor1_on = false;
  static bool motor2_on = false;
  static bool motor3_on = false;
  static bool motor4_on = false;
  static uint8_t motor_speed = MOTOR_ZERO_SPEED;

  accel_read(&kami_drone.accel_data);
  gyro_read(&kami_drone.gyro_data);

  /*
  display.setCursor(0,0);
  display.println(kami_drone.roll);
  display.setCursor(0,10);
  display.println(kami_drone.pitch);
  display.display();
  display.clearDisplay();
  */

  if (print_stuff) {
    USB_SERIAL.print(kami_drone.roll);
    USB_SERIAL.print(" ");
    USB_SERIAL.print(kami_drone.pitch);
    USB_SERIAL.print("\n");
  }

  if (USB_SERIAL.available() > 0) {

    uint8_t rec_byte = USB_SERIAL.read();
    switch(rec_byte) {
      case '1':
        motor1_on = !motor1_on;
        break;

      case '2':
        motor2_on = !motor2_on;
        break;

      case '3':
        motor3_on = !motor3_on;
        break;

      case '4':
        motor4_on = !motor4_on;
        break;

      case 'a':
        WIFI_SERIAL.print("AT\r\n");
        break;

      case 'r':
        wifi_sendCommand(&WIFI_SERIAL, AT_RESET);
        break;

      case 't':
        wifi_sendCommand(&WIFI_SERIAL, AT_QUERY_TIMEOUT);
        break;

      case 'T':
        wifi_sendCommand(&WIFI_SERIAL, AT_SET_TIMEOUT);
        break;

      case 's':
        wifi_sendCommand(&WIFI_SERIAL, AT_STATUS);
        break;

      case 'w':
        wifi_sendCommand(&WIFI_SERIAL, AT_AP_MODE);
        delay(1000);
        wifi_sendCommand(&WIFI_SERIAL, AT_CREAT_AP);
        delay(1000);
        wifi_sendCommand(&WIFI_SERIAL, AT_MULTI_CONN);
        delay(1000);
        wifi_sendCommand(&WIFI_SERIAL, AT_CREAT_SERVER);
        delay(1000);
        break;

      case 'e':
        wifi_sendCommand(&WIFI_SERIAL, AT_ECHO_OFF);
        break;

      case 'E':
        wifi_sendCommand(&WIFI_SERIAL, AT_ECHO_ON);
        break;

      case 'q':
        wifi_sendCommand(&WIFI_SERIAL, AT_QUERY_AP);
        break;

      case 'Q':
        wifi_sendCommand(&WIFI_SERIAL, AT_GET_CLIENTS);
        break;

      case 'p':
        print_stuff = !print_stuff;
        break;

      case 'P':
        USB_SERIAL.println(kami_drone.accel_data.x_off); //-3, -5 62 -50 21 -14
        USB_SERIAL.println(kami_drone.accel_data.y_off);
        USB_SERIAL.println(kami_drone.accel_data.z_off);

        USB_SERIAL.println(kami_drone.gyro_data.roll_dot_off);
        USB_SERIAL.println(kami_drone.gyro_data.pitch_dot_off);
        USB_SERIAL.println(kami_drone.gyro_data.yaw_dot_off);
        break;

      case '+':
        motor_speed += 5;
        USB_SERIAL.println(motor_speed);
        break;

      case '-':
        motor_speed -= 5;
        USB_SERIAL.println(motor_speed);
        break;

      case 'M':
        motor_speed = 255;
        USB_SERIAL.println(motor_speed);
        break;

      case 'm':
        motor_speed = 0;
        USB_SERIAL.println(motor_speed);
        break;

      case 'z':
        motor_speed = MOTOR_ZERO_SPEED;
        USB_SERIAL.println(motor_speed);
        break;
    }
  }

  if (motor1_on) analogWrite(MOTOR_PIN_1, motor_speed);
  else analogWrite(MOTOR_PIN_1, MOTOR_ZERO_SPEED);

  if (motor2_on) analogWrite(MOTOR_PIN_2, motor_speed);
  else analogWrite(MOTOR_PIN_2, MOTOR_ZERO_SPEED);

  if (motor3_on) analogWrite(MOTOR_PIN_3, motor_speed);
  else analogWrite(MOTOR_PIN_3, MOTOR_ZERO_SPEED);

  if (motor4_on) analogWrite(MOTOR_PIN_4, motor_speed);
  else analogWrite(MOTOR_PIN_4, MOTOR_ZERO_SPEED);

  while (WIFI_SERIAL.available() > 0) {
    uint8_t rec_byte = WIFI_SERIAL.read();
#ifdef WIFI_DEBUG
    USB_SERIAL.write(rec_byte);
#endif
  }
  delay(10);
}

extern "C" int main(void) {
  pinMode(13, OUTPUT);
  pinMode(TEST_PIN1, OUTPUT);
  pinMode(TEST_PIN2, OUTPUT);
  digitalWriteFast(13, HIGH);
  delay(500);
  digitalWriteFast(13, LOW);
  delay(500);
  setup();
  while (1) {
    loop();
    yield();
  }
}
