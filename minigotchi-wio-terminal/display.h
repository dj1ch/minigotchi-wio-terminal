/**
 * display.h: header files for display.cpp
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "config.h"
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <Wire.h>

class Display {
public:
  ~Display();
  static void startScreen();
  static void updateDisplay(String face);
  static void updateDisplay(String face, String text);

private:
  static TFT_eSPI *tft_display;
};

#endif // DISPLAY_H
