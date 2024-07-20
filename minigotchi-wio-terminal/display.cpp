/**
 * display.cpp: handles display support
 */

#include "display.h"

TFT_eSPI *Display::tft_display = nullptr;

/**
 * Deletes any pointers if used
 */
Display::~Display() {

}

/**
 * Function to initialize the screen ONLY.
 */
void Display::startScreen() {
  if (Config::display) {
    
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
    
  }
}
