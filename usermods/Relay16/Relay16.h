// Relay16.h
// Relay16 usermod for WLED

#pragma once // add this line

#include <WLED.h>

#define MAX_RELAYS 16 // Define the maximum number of relays you want to control

// relay pins
const uint8_t relayPins[MAX_RELAYS] = {
    4,  // D4
    5,  // D5
    12, // D12 L7
    13, // D13 L8
    14, // D14 L6
    15, // D15
    17, // TX2
    18, // SCK
    19, // MISO
    21, // SDA
        // SCL
    23, // MOSI
    25, // L3
    26, // L4
    27, // L5
    32, // L 1
    33  // L2
};

class Relay16 : public Usermod
{
public:
  // constructor
  Relay16();

  // destructor
  ~Relay16();

  // getter method for relay pin
  int8_t getRelayPin(uint8_t index);

  // setter method for relay pin
  void setRelayPin(uint8_t index, uint8_t pin);

  // getter method for relay state
  bool getRelayState(uint8_t index);

  // setter method for relay state
  void setRelayState(uint8_t index, bool state);

  // toggle a relay
  void toggleRelay(uint8_t index);

  void renameSwitch(AsyncWebServerRequest &request);

  void addToPage(AsyncWebServerRequest &request);
  // usermod methods
  void setup() override;
  void loop() override;
  bool handleButton(uint8_t buttonIdx) override;
  void addToJsonState(JsonObject &root) override;
  void readFromJsonState(JsonObject &root) override;
  void addToJsonInfo(JsonObject &root) override;
  void addToConfig(JsonObject &root) override;
  bool readFromConfig(JsonObject &root) override;

  struct Relay
  {
    uint8_t pin;
    bool state;
    String name;
  };

  Relay _relays[MAX_RELAYS];

  // current relay index
};
