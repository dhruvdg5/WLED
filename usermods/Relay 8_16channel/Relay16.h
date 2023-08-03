// Relay16.h
// Relay16 usermod for WLED

#pragma once // add this line

#ifndef WLED_ENABLE_USERMOD_RELAY16 // change this line
#error "To use usermod Relay16, please go to Tools -> WLED Defines and uncomment #define WLED_ENABLE_USERMOD_RELAY16"
#endif

#include <WLED.h>

#define MAX_RELAYS 16 // Define the maximum number of relays you want to control


#define LED_PIN 16 // Define the pin number for the LED data pin


//relay pins
const int relayPins[MAX_RELAYS] = {
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



class Relay16 : public Usermod
 {

  public:
    // constructor
Relay16() {
  // initialize any variables or resources here
  // for example, you can set the default state of the relays to off
  for (int i = 0; i < MAX_RELAYS; i++) {
    _relays[i].state = false;
  }
}
// destructor
~Relay16() {
  // free any resources here
  // for example, you can turn off all the relays before destroying the object
  for (int i = 0; i < MAX_RELAYS; i++) {
    digitalWrite(_relays[i].pin, LOW);
  }
}



// getter method for relay pin
int getRelayPin(int index) {
  // check if the index is valid
  if (index >= 0 && index < MAX_RELAYS) {
    // return the pin number of the relay at the given index
    return _relays[index].pin;
  } else {
    // return -1 if the index is invalid
    return -1;
  }
}

// setter method for relay pin
void setRelayPin(int index, int pin) {
  // check if the index and pin are valid
  if (index >= 0 && index < MAX_RELAYS)
   {
    // set the pin number of the relay at the given index
    _relays[index].pin = pin;
    // set the pin mode as output
    pinMode(pin, OUTPUT);
  } else {
    // do nothing if the index or pin are invalid
  }
}

// getter method for relay state
bool getRelayState(int index) {
  // check if the index is valid
  if (index >= 0 && index < MAX_RELAYS) {
    // return the state of the relay at the given index
    return _relays[index].state;
  } else {
    // return false if the index is invalid
    return false;
  }
}

// setter method for relay state
void setRelayState(int index, bool state) {
  // check if the index is valid
  if (index >= 0 && index < MAX_RELAYS) {
    // set the state of the relay at the given index
    _relays[index].state = state;
    // write the state to the pin
    digitalWrite(_relays[index].pin, state ? HIGH : LOW);
  } else {
    // do nothing if the index is invalid
  }
}

void toggleRelay(uint8_t index); // change this line

private:
  struct Relay {
    int pin;
    bool state;
  };

  Relay _relays[MAX_RELAYS];
  // declare the variables here
  unsigned long lastTime = 0; // last time the relay was toggled
  unsigned long toggleInterval = 1000; // toggle interval in milliseconds
  uint8_t currentRelay = 0; // current relay index

public:
  void setup() override;
  void loop() override;
  bool handleButton(uint8_t buttonIdx) override;
  void addToJsonState(JsonObject &root) override;
  void readFromJsonState(JsonObject &root) override;
  void addToJsonInfo(JsonObject &root) override;
  void addToConfig(JsonObject &root) override;
  bool readFromConfig(JsonObject &root) override;
};
