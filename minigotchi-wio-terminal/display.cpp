/**
 * display.cpp: handles display support
 */

#include "display.h"

// the only screen...
TFT_eSPI tft;
TFT_eSPI *Display::tft_display = nullptr;

String Display::storedFace = "";
String Display::previousFace = "";

String Display::storedText = "";
String Display::previousText = "";


/**
 * Deletes any pointers if used
 */
Display::~Display() {
  if (tft_display) {
    delete tft_display;
  }
}

/**
 * Function to initialize the screen ONLY.
 */
void Display::startScreen() {
  if (Config::display) {
      tft_display = &tft;
      tft.begin();
      tft.setRotation(1);
      delay(100);
  }
}

/**
 * Updates the face ONLY
 * @param face Face to use
 */
void Display::updateDisplay(String face) { Display::updateDisplay(face, ""); }

/**
 * Updates the display with both face and text
 * @param face Face to use
 * @param text Additional text under the face
 */
void Display::updateDisplay(String face, String text) {
  if (Config::display) {
    bool faceChanged = (face != Display::storedFace);
    bool textChanged = (text != Display::storedText);

    if (faceChanged) {
      int faceHeight = 40;
      tft.fillRect(0, 0, tft.width(), faceHeight,
                  TFT_BLACK);
      tft.setCursor(0, 5);
      tft.setTextSize(4);
      tft.setTextColor(TFT_WHITE);
      tft.println(face);
      Display::storedFace = face;
    }

    if (textChanged) {
      int textY = 40;
      tft.fillRect(0, textY, tft.width(), tft.height() - textY,
                    TFT_BLACK);
      tft.setCursor(0, textY);
      tft.setTextSize(1);
      tft.setTextColor(TFT_WHITE);
      tft.println(text);
      Display::storedText = text;
    }
  }
}
