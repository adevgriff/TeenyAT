#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "teenyat.h"

/*
 * Connect the following pins from your I2C OLED SCREEN
 * GND - Pico Pin 38 Ground Pin
 * VCC - Pico Pin 36 3v3(OUT)
 * SDA - Pico Pin 6 GPIO pin 4
 * SCL - Pico Pin 7 GPIO pin 5
 */

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

void appendColumn(uint16_t value);

/* Declaration for an SSD1306 display connected to I2C (SDA, SCL pins) */
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

teenyat tnyAt;

void bus_read(teenyat *t, tny_uword addr, tny_word *data, uint16_t *delay) {
  return;
}

void bus_write(teenyat *t, tny_uword addr, tny_word data, uint16_t *delay) {
  if(addr > 0x7999) {
    appendColumn(data.u);
  }
  return;
}

void setup()
{
  /*Initialize with the I2C addr 0x3C (for the 128x32)*/
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  display.clearDisplay();

  /*Create TnyAt and Test*/
  tny_init_from_file(&tnyAt, bus_read, bus_write);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Welcome TNY");
  display.display();
  delay(2000);
}

void loop()
{
  tny_clock(&tnyAt);
}

void appendColumn(uint16_t value) {
  /*Shift everything over two 2 pixel hight need not be considered here*/
  for(int x = 2; x < SCREEN_WIDTH; x++) {
    for(int y = 0; y < SCREEN_HEIGHT; y++) {
      uint16_t pixel_state = display.getPixel(x, y);
      display.drawPixel(x - 2, y, pixel_state);
    }
  }

  /*Clear last column*/
  for(int x = 1; x <= 2; x++) {
    for(int y = 0; y < SCREEN_HEIGHT; y++) {
      display.drawPixel(SCREEN_WIDTH - x, y, SSD1306_BLACK);
    }
  }

  /*Set 2x2 pixel if bit is set in that position*/
  for(int i = 0; i < SCREEN_HEIGHT/2; i++) {
    if((value >> i) & 0b1) {
      display.drawPixel(SCREEN_WIDTH - 1, i * 2, SSD1306_WHITE);
      display.drawPixel(SCREEN_WIDTH - 2, i * 2, SSD1306_WHITE);
      display.drawPixel(SCREEN_WIDTH - 1, (i * 2) + 1, SSD1306_WHITE);
      display.drawPixel(SCREEN_WIDTH - 2, (i * 2) + 1, SSD1306_WHITE);
    }
  }

  display.display();
}