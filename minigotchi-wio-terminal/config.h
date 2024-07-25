/**
 * config.h: header files for config.cpp
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "minigotchi.h"
#include <Arduino.h>
#include <vector>
#include <string>

class Config {
public:
  static bool deauth;
  static bool advertise;
  static bool scan;
  static int shortDelay;
  static int longDelay;
  static bool parasite;
  static bool display;
  static String screen;
  static int baud;
  static int channel;
  static std::vector<std::string> whitelist;
  static int epoch;
  static String grid_version;
  static String face;
  static String identity;
  static String name;
  static int ap_ttl;
  static bool associate;
  static int bored_num_epochs;
  static int channels[13];
  static int excited_num_epochs;
  static int hop_recon_time;
  static int max_inactive_scale;
  static int max_interactions;
  static int max_misses_for_recon;
  static int min_recon_time;
  static int min_rssi;
  static int recon_inactive_multiplier;
  static int recon_time;
  static int sad_num_epochs;
  static int sta_ttl;
  static int pwnd_run;
  static int pwnd_tot;
  static String session_id;
  static int timestamp;
  static int uptime;
  static String version;

private:
  static int random(int min, int max);
  static int time();
};

#endif // CONFIG_H
