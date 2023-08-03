// Relay16.h
// Relay16 usermod for WLED

#pragma once // add this line

#ifndef WLED_ENABLE_USERMOD_RELAY16 // change this line
#error "To use usermod Relay16, please go to Tools -> WLED Defines and uncomment #define WLED_ENABLE_USERMOD_RELAY16"
#endif

#include <WLED.h>

#define MAX_RELAYS 16 // Define the maximum number of relays you want to control

//relay pins
const uint8_t relayPins[MAX_RELAYS] = {
  4, // D4
  5, // D5
  12, // D12
  13, // D13
  14, // D14
  15, // D15
  17, // TX2
  18, // SCK
  19, // MISO
  21, // SDA
  22, // SCL
  23, // MOSI
  25, // DAC_1
  26, // DAC_2
  27, // D27
  32 // D32
};

class Relay16 : public Usermod {
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

  void renameSwitch(AsyncWebServer* server);
  void addToPage(AsyncWebServer* server);
  // usermod methods
  void setup() override;
  void loop() override;
  bool handleButton(uint8_t buttonIdx) override;
  void addToJsonState(JsonObject &root) override;
  void readFromJsonState(JsonObject &root) override;
  void addToJsonInfo(JsonObject &root) override;
  void addToConfig(JsonObject &root) override;
  bool readFromConfig(JsonObject &root) override;

private:
  struct Relay {
    uint8_t pin;
    bool state;
    String name;
  };

  Relay _relays[MAX_RELAYS];

  // declare the variables here
  unsigned long lastTime = 0;            // last time the relay was toggled
  unsigned long toggleInterval = 1000;   // toggle interval in milliseconds
  uint8_t currentRelay = 0;              // current relay index
};

