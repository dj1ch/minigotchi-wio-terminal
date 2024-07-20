/**
 * pwnagotchi.h: header files for pwnagotchi.cpp
 */

#ifndef PWNAGOTCHI_H
#define PWNAGOTCHI_H

#include "frame.h"
#include "minigotchi.h"
#include "parasite.h"
#include "wifi.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <stdint.h>
#include <string>
#include <WString.h>

extern "C" {
#include "structs.h"
}

class Pwnagotchi {
public:
  static void detect();
  static void pwnagotchiCallback(unsigned char *buf, short unsigned int len);
  static void processJson(DynamicJsonDocument &jsonBuffer);
  static void stopCallback();

private:
  static String findCopy(const String& buf);
  static String extractMAC(const unsigned char *buff);
  static void getMAC(char *addr, const unsigned char *buff, int offset);
  static bool parsed;
  static bool pwnagotchiDetected;
};

#endif // PWNAGOTCHI_H
