/**
 * deauth.h: header files for deauth.cpp
 */

#ifndef DEAUTH_H
#define DEAUTH_H

#include "config.h"
#include "minigotchi.h"
#include "parasite.h"
#include <Arduino.h>
#include "wifi.h"
#include "rpcWiFi.h"

class Deauth {
public:
  static void deauth();
  static uint8_t deauthTemp[26];
  static uint8_t deauthFrame[26];
  static uint8_t disassociateFrame[26];
  static uint8_t broadcastAddr[6];
  static int randomIndex;

private:
  static void send(uint8_t *buf, uint16_t len);
  static bool broadcast(uint8_t *mac);
  static String printHidden(int network);
  static void printMac(uint8_t *mac);
  static String printMacStr(uint8_t *mac);
  static bool select();
  static void start();
  static uint8_t bssid[6];
  static bool running;
  static String randomAP;
};

#endif // DEAUTH_H
