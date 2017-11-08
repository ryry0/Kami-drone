#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ring_buffer.h"
#include "teensy_accel.h"
#include "teensy_gyro.h"

#define OLED_RESET 15
#define I2C_SDA 34
#define I2C_SCL 33

struct ring_buffer_s ring_buff;

Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

typedef struct kami_drone_s {
  accel_data_t accel_data;
  gyro_data_t gyro_data;
} kami_drone_t;

volatile kami_drone_t kami_drone;

void setup() {
  //-----------------
  Serial.begin(9600);
  Wire.setSDA(I2C_SDA);
  Wire.setSCL(I2C_SCL);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  //-----------------
  accel_init();
  gyro_init();
  accel_calibrate(&kami_drone.accel_data);
  gyro_calibrate(&kami_drone.gyro_data);
}

void loop() {
  static int i = 0;
  digitalWriteFast(35, HIGH);
  accel_read(&kami_drone.accel_data);
  gyro_read(&kami_drone.gyro_data);
  digitalWriteFast(35, LOW);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(kami_drone.accel_data.raw_z);
  display.setCursor(0,10);
  display.println(kami_drone.gyro_data.raw_roll_dot);
  display.setCursor(0,20);
  display.println(i++, HEX);
  display.display();
  display.clearDisplay();
}

extern "C" int main(void) {
  pinMode(13, OUTPUT);
  pinMode(35, OUTPUT);
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
