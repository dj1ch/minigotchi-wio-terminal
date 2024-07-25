/**
 * frame.cpp: handles the sending of "pwnagotchi" beacon frames
 */

#include "frame.h"

/** developer note:
 *
 * when it comes to detecting a pwnagotchi, this is done with pwngrid/opwngrid.
 * essentially pwngrid looks for the numbers 222-226 in payloads, and if they
 * aren't there, it ignores it. these need to be put into the frames!!!
 *
 * note that these frames aren't just normal beacon frames, rather a modified
 * one with data, additional ids, etc. frames are dynamically constructed,
 * headers are included like a normal frame. by far this is the most memory
 * heaviest part of the minigotchi, the reason is
 *
 */

// initializing
size_t Frame::payloadSize = 255; // by default
const size_t Frame::chunkSize = 0xFF;
bool Frame::sent = false;

// beacon stuff
size_t Frame::essidLength = 0;
uint8_t Frame::headerLength = 0;

// payload ID's according to pwngrid
const uint8_t Frame::IDWhisperPayload = 0xDE;
const uint8_t Frame::IDWhisperCompression = 0xDF;
const uint8_t Frame::IDWhisperIdentity = 0xE0;
const uint8_t Frame::IDWhisperSignature = 0xE1;
const uint8_t Frame::IDWhisperStreamHeader = 0xE2;

// other addresses
const uint8_t Frame::SignatureAddr[] = {0xde, 0xad, 0xbe, 0xef, 0xde, 0xad};
const uint8_t Frame::BroadcastAddr[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
const uint16_t Frame::wpaFlags = 0x0411;

// Don't even dare restyle!
const uint8_t Frame::header[]{
    /*  0 - 1  */ 0x80,
    0x00, // frame control, beacon frame
    /*  2 - 3  */ 0x00,
    0x00, // duration
    /*  4 - 9  */ 0xff,
    0xff,
    0xff,
    0xff,
    0xff,
    0xff, // broadcast address
    /* 10 - 15 */ 0xde,
    0xad,
    0xbe,
    0xef,
    0xde,
    0xad, // source address
    /* 16 - 21 */ 0xde,
    0xad,
    0xbe,
    0xef,
    0xde,
    0xad, // bssid
    /* 22 - 23 */ 0x00,
    0x00, // fragment and sequence number
    /* 24 - 32 */ 0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00, // timestamp
    /* 33 - 34 */ 0x64,
    0x00, // interval
    /* 35 - 36 */ 0x11,
    0x04, // capability info
};

// get header length
const int Frame::pwngridHeaderLength = sizeof(Frame::header);

/** developer note:
 *
 * frame structure based on how it was built here
 *
 * 1. header
 * 2. payload id's
 * 3. (chunked) pwnagotchi data
 *
 */

/** developer note:
 *
 * referenced the following for packing-related function:
 *
 * https://github.com/evilsocket/pwngrid/blob/master/wifi/pack.go
 *
 */

/**
 * Replicates pwngrid's pack() function from pack.go
 * https://github.com/evilsocket/pwngrid/blob/master/wifi/pack.go
 */
uint8_t *Frame::pack() {
  // make a json doc
  String jsonString = "";
  DynamicJsonDocument doc(2048);

  doc["epoch"] = Config::epoch;
  doc["face"] = Config::face;
  doc["identity"] = Config::identity;
  doc["name"] = Config::name;

  doc["policy"]["advertise"] = Config::advertise;
  doc["policy"]["ap_ttl"] = Config::ap_ttl;
  doc["policy"]["associate"] = Config::associate;
  doc["policy"]["bored_num_epochs"] = Config::bored_num_epochs;

  doc["policy"]["deauth"] = Config::deauth;
  doc["policy"]["excited_num_epochs"] = Config::excited_num_epochs;
  doc["policy"]["hop_recon_time"] = Config::hop_recon_time;
  doc["policy"]["max_inactive_scale"] = Config::max_inactive_scale;
  doc["policy"]["max_interactions"] = Config::max_interactions;
  doc["policy"]["max_misses_for_recon"] = Config::max_misses_for_recon;
  doc["policy"]["min_recon_time"] = Config::min_rssi;
  doc["policy"]["min_rssi"] = Config::min_rssi;
  doc["policy"]["recon_inactive_multiplier"] =
      Config::recon_inactive_multiplier;
  doc["policy"]["recon_time"] = Config::recon_time;
  doc["policy"]["sad_num_epochs"] = Config::sad_num_epochs;
  doc["policy"]["sta_ttl"] = Config::sta_ttl;

  doc["pwnd_run"] = Config::pwnd_run;
  doc["pwnd_tot"] = Config::pwnd_tot;
  doc["session_id"] = Config::session_id;
  doc["uptime"] = Config::uptime;
  doc["version"] = Config::version;

  // serialize then put into beacon frame
  serializeJson(doc, jsonString);
  Frame::essidLength = measureJson(doc);
  Frame::headerLength = 2 + ((uint8_t)(essidLength / 255) * 2);

  uint8_t *beaconFrame = new uint8_t[Frame::pwngridHeaderLength +
                                     Frame::essidLength + Frame::headerLength];
  memcpy(beaconFrame, Frame::header, Frame::pwngridHeaderLength);

  /** developer note:
   *
   * if you literally want to check the json everytime you send a packet(non
   * serialized ofc)
   *
   * Serial.println(jsonString);
   */

  int frameByte = pwngridHeaderLength;
  for (int i = 0; i < essidLength; i++) {
    if (i == 0 || i % 255 == 0) {
      beaconFrame[frameByte++] = Frame::IDWhisperPayload;
      uint8_t newPayloadLength = 255;
      if (essidLength - i < Frame::chunkSize) {
        newPayloadLength = essidLength - i;
      }
      beaconFrame[frameByte++] = newPayloadLength;
    }
    beaconFrame[frameByte++] = (uint8_t)jsonString[i];
  }

  /* Uncomment if you want to test beacon frames

  Serial.println("('-') Full Beacon Frame:");
  for (size_t i = 0; i < sizeof(beaconFrame); ++i) {
    Serial.print(beaconFrame[i], HEX);
    Serial.print(" ");
  }
  Serial.println(" ");

  */

  return beaconFrame;
}

/**
 * Sends a pwnagotchi packet in AP mode
 */
void Frame::send() {
  // build frame
  WiFi.mode(WIFI_AP);
  uint8_t *frame = Frame::pack();
  size_t frameSize = Frame::pwngridHeaderLength + Frame::essidLength +
                     Frame::headerLength; // actually disgusting but it works

  // send full frame
  // we dont use raw80211 since it sends a header(which we don't need), although
  // we do use it for monitoring, etc.
  wifi_tx_raw_frame(frame, frameSize);
  delay(102);

  delete[] frame;
}

/**
 * Full usage of Pwnagotchi's advertisments on the Minigotchi.
 */
void Frame::advertise() {
  int packets = 0;
  unsigned long startTime = millis();

  if (Config::advertise) {
    Serial.println("(>-<) Starting advertisment...");
    Serial.println(" ");
    Display::updateDisplay("(>-<)", "Starting advertisment...");
    Minigotchi::monStart();
    Parasite::sendAdvertising();
    delay(Config::shortDelay);
    for (int i = 0; i < 150; ++i) {
      Frame::send();  
      packets++;

      // calculate packets per second
      float pps = packets / (float)(millis() - startTime) * 1000;

      // show pps
      if (!isinf(pps)) {
        Serial.print("(>-<) Packets per second: ");
        Serial.print(pps);
        Serial.print(" pkt/s (Channel: ");
        Serial.println(")");
        Display::updateDisplay(
            "(>-<)", "Packets per second: " + (String)pps + " pkt/s" +
                          "(Channel: " + ")");
      }
    }

    Serial.println(" ");
    Serial.println("(^-^) Advertisment finished!");
    Serial.println(" ");
    Display::updateDisplay("(^-^)", "Advertisment finished!");
  } else {
    // do nothing but still idle
  }
}
