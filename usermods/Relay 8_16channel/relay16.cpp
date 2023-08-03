#pragma once
#include "wled.h"

class Relay16 : public Usermod {
  private:
    // declare your variables and constants here
    const uint8_t relayPins[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    bool relayStates[16] = {false};
        bool initDone = false;
  
  public:
    // override the virtual methods here
    void setup() {
      //Serial.println("Hello from my usermod!");
      initDone = true;
      // initialize your relay pins as outputs
      for (int i = 0; i < 16; i++) {
        pinMode(relayPins[i], OUTPUT);
        digitalWrite(relayPins[i], LOW);
      }
    }

    void loop() {
      // update your relay states based on some logic or input
      // for example, you can use the WLED API or MQTT to control the relays
      // or you can use some sensors or buttons connected to the ESP32 board
      // or you can use some timer or schedule logic
      // or anything else you want
      // just make sure to update the relayStates array accordingly
      // and write the corresponding values to the ESP32 board pins
      for (int i = 0; i < 16; i++) {
        digitalWrite(relayPins[i], relayStates[i] ? HIGH : LOW);
      }
    }


    void connected() {
      //Serial.println("Connected to WiFi!");
    }

    void addToJsonInfo(JsonObject& root) {
      // add some information about your usermod to the WLED info page
      // for example, you can add the relay states as an array
      JsonArray arr = root.createNestedArray("Relay States");
      for (int i = 0; i < 16; i++) {
        arr.add(relayStates[i]);
      }
    }

    void readFromJsonState(JsonObject& root) {
      // read some state from the WLED JSON API
      // for example, you can read the relay states as an array
      if (root.containsKey("Relay States")) {
        JsonArray arr = root["Relay States"];
        for (int i = 0; i < 16; i++) {
          relayStates[i] = arr[i];
        }
      }
    }
};

