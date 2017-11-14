#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ring_buffer.h"
#include "teensy_accel.h"
#include "teensy_gyro.h"
#include "teensy_esp8266.h"
#include "numerical.h"

#define OLED_RESET 15
#define I2C_SDA 34
#define I2C_SCL 33

#define TEST_PIN1 35
#define TEST_PIN2 36

#define FIL_ALPHA 0.995
#define DELTA_TIME 0.002
#define LOOP_CLOSURE_US 2000

#define I2C_FASTMODE 400000

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

IntervalTimer control_timer;
Adafruit_SSD1306 display(OLED_RESET);


typedef struct kami_drone_s {
  accel_data_t accel_data;
  gyro_data_t gyro_data;
  float roll;
  float pitch;
} kami_drone_t;

volatile kami_drone_t kami_drone;

void setupSerial() {
  Serial.begin(9600);
  Serial1.begin(115200);
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
  wifi_sendCommand(&Serial1, "ATE0\r\n");

}

void setup() {
  //-----------------
  setupSerial();
  setupWire();
  setupDisplay();

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
    Serial.print(kami_drone.roll);
    Serial.print(" ");
    Serial.print(kami_drone.pitch);
    Serial.print("\n");
  }

  if (Serial.available() > 0) {

    uint8_t rec_byte = Serial.read();
    switch(rec_byte) {
      case 'a':
        Serial1.print("AT\r\n");
        break;

      case 'r':
        wifi_sendCommand(&Serial1, AT_RESET);
        break;

      case 'w':
        wifi_sendCommand(&Serial1, AT_CREAT_AP);
        delay(1000);
        wifi_sendCommand(&Serial1, AT_ACCEPT_CONNS);
        delay(1000);
        wifi_sendCommand(&Serial1, AT_MULTI_CONN);
        delay(1000);
        wifi_sendCommand(&Serial1, AT_CREAT_SERVER);
        delay(1000);
        break;

      case 'e':
        wifi_sendCommand(&Serial1, AT_ECHO_OFF);
        break;

      case 'E':
        wifi_sendCommand(&Serial1, AT_ECHO_ON);
        break;

      case 'q':
        wifi_sendCommand(&Serial1, AT_QUERY_AP);
        break;

      case 'Q':
        wifi_sendCommand(&Serial1, AT_GET_CLIENTS);
        break;

      case 'p':
        print_stuff = !print_stuff;
        break;
    }
  }

  while (Serial1.available() > 0) {
    uint8_t rec_byte = Serial1.read();
    Serial.write(rec_byte);
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
