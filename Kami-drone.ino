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
#include "packet_drone.h"

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

/***************************************************************************************/
typedef struct mtr_drone_s {
  bool enabled;
  uint8_t pin;
  uint8_t speed;
} mtr_drone_t;

void mtr_setSpeed(volatile mtr_drone_t *mtr, uint8_t speed) {
  if (mtr->enabled)
    mtr->speed = speed;
  else
    mtr->speed = MOTOR_ZERO_SPEED;

  analogWrite(mtr->pin, mtr->speed);
}

void mtr_init(volatile mtr_drone_t *mtr, uint8_t pin) {
  mtr->pin = pin;
  mtr_disable(mtr);
  mtr_setSpeed(mtr, MOTOR_ZERO_SPEED);
}

void mtr_toggle(volatile mtr_drone_t *mtr) {
  mtr->enabled = !mtr->enabled;
}

void mtr_enable(volatile mtr_drone_t *mtr) {
  mtr->enabled = true;
}

void mtr_disable(volatile mtr_drone_t *mtr) {
  mtr->enabled = false;
}

/***************************************************************************************/

typedef struct kami_drone_s {
  accel_data_t accel_data;
  gyro_data_t gyro_data;
  float roll;
  float pitch;
  uint8_t debug_motor_speed;
  bool usb_print;
  pkt_generic_t packet;
  mtr_drone_t motor1;
  mtr_drone_t motor2;
  mtr_drone_t motor3;
  mtr_drone_t motor4;
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
  //wifi_sendCommand(&WIFI_SERIAL, "ATE0\r\n");

}

void setupDrone() {
  kami_drone.usb_print = false;
  kami_drone.roll = 0;
  kami_drone.pitch = 0;
  kami_drone.debug_motor_speed = MOTOR_ZERO_SPEED;
  mtr_init(&kami_drone.motor1, MOTOR_PIN_1);
  mtr_init(&kami_drone.motor2, MOTOR_PIN_2);
  mtr_init(&kami_drone.motor3, MOTOR_PIN_3);
  mtr_init(&kami_drone.motor4, MOTOR_PIN_4);
  pkt_init((pkt_generic_t *) &kami_drone.packet);
}

void setup() {
  //-----------------
  setupSerial();
  setupWire();
  setupPWM();
  setupDrone();
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
    handleKeyCommands((kami_drone_t *) &kami_drone, rec_byte);
  }

  mtr_setSpeed(&kami_drone.motor1, kami_drone.debug_motor_speed);
  mtr_setSpeed(&kami_drone.motor2, kami_drone.debug_motor_speed);
  mtr_setSpeed(&kami_drone.motor3, kami_drone.debug_motor_speed);
  mtr_setSpeed(&kami_drone.motor4, kami_drone.debug_motor_speed);

  handleWifi((kami_drone_t *) &kami_drone);
  delay(10);
}

void handleWifi(struct kami_drone_s *kami_drone) {
  while (WIFI_SERIAL.available() > 0) {
    uint8_t input_byte = WIFI_SERIAL.read();
    pkt_decodeByteHandler(&kami_drone->packet, input_byte, handlePacket);
#ifdef WIFI_DEBUG
    USB_SERIAL.write(input_byte);
#endif
  }
}

void handlePacket(pkt_generic_t *packet) {
  USB_SERIAL.print("tengo packet\n");
  if (packet->type == PKT_DATA_START)
    USB_SERIAL.print("weeener\n");
}

void handleKeyCommands(struct kami_drone_s *kami_drone, uint8_t rec_byte) {
  switch(rec_byte) {
    case '1':
      mtr_toggle(&kami_drone->motor1);
      break;

    case '2':
      mtr_toggle(&kami_drone->motor2);
      break;

    case '3':
      mtr_toggle(&kami_drone->motor3);
      break;

    case '4':
      mtr_toggle(&kami_drone->motor4);
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
      kami_drone->usb_print = !kami_drone->usb_print;
      break;

    case 'P':
      USB_SERIAL.println(kami_drone->accel_data.x_off); //-3, -5 62 -50 21 -14
      USB_SERIAL.println(kami_drone->accel_data.y_off);
      USB_SERIAL.println(kami_drone->accel_data.z_off);

      USB_SERIAL.println(kami_drone->gyro_data.roll_dot_off);
      USB_SERIAL.println(kami_drone->gyro_data.pitch_dot_off);
      USB_SERIAL.println(kami_drone->gyro_data.yaw_dot_off);
      break;

    case '+':
      kami_drone->debug_motor_speed += 5;
      USB_SERIAL.println(kami_drone->debug_motor_speed);
      break;

    case '-':
      kami_drone->debug_motor_speed -= 5;
      USB_SERIAL.println(kami_drone->debug_motor_speed);
      break;

    case 'M':
      kami_drone->debug_motor_speed = 255;
      USB_SERIAL.println(kami_drone->debug_motor_speed);
      break;

    case 'm':
      kami_drone->debug_motor_speed = 0;
      USB_SERIAL.println(kami_drone->debug_motor_speed);
      break;

    case 'z':
      kami_drone->debug_motor_speed = MOTOR_ZERO_SPEED;
      USB_SERIAL.println(kami_drone->debug_motor_speed);
      break;
  }
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

