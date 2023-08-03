#ifndef Relay16_h
#define Relay16_h

#include <WLED.h>

#define MAX_RELAYS 16 // Define the maximum number of relays you want to control

class Relay16 : public Usermod
 {
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

void toggleRelay(int index) {
  // check if the index is valid
  if (index >= 0 && index < MAX_RELAYS) {
    // get the current state of the relay at the given index
    bool state = getRelayState(index);
    // set the new state to be the opposite of the current state
    setRelayState(index, !state);
  } else {
    // do nothing if the index is invalid
  }
}
private:
  struct Relay {
    int pin;
    bool state;
  };

  Relay _relays[MAX_RELAYS];

public:

  // setup function that runs once at boot
void setup() override;
// loop function that runs every loop cycle
void loop() override;
// button handler function that runs when a button is pressed
// returns true if the button press was handled, false otherwise
bool handleButton(uint8_t buttonIdx) override;
// JSON state function that adds usermod data to the JSON object
void addToJsonState(JsonObject &root) override;
// JSON state function that reads usermod data from the JSON object
void readFromJsonState(JsonObject &root) override;
// JSON info function that adds usermod info to the JSON object
void addToJsonInfo(JsonObject &root) override;
// config function that adds usermod settings to the config file
void addToConfig(JsonObject &root) override;
// config function that reads usermod settings from the config file
// returns true if successful, false otherwise
bool readFromConfig(JsonObject &root) override;a

  // this function is used to identify your usermod. It should return a unique identifier for your usermod.
    uint16_t getId() {
      return USERMOD_ID_MY_USERMOD; // define this macro in const.h
    }

    // this function is used to name your usermod. It should return a descriptive name for your usermod.
    String getName() {
      return F("My Usermod"); // F macro to reduce RAM usage
    }

    
    // this function is called every time WiFi is (re)connected. You can use it to sync time or get data from the internet.
    void connected() {
      // do some network-related stuff here, such as getting NTP time, weather data, etc.
    }

    // this function is called every time WiFi is disconnected. You can use it to handle network loss gracefully.
    void disconnected() {
      // do some network-related stuff here, such as setting a fallback mode, etc.
    }  // this function is called once at boot. You can use it to initialize your usermod.
   

};

#endif  
 