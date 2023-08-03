// Relay16.cpp
// Relay16 usermod for WLED

#include "Relay16.h"

// constructor
Relay16::Relay16() {
  // initialize any variables or resources here
  // for example, you can set the default state of the relays to off
  for (int i = 0; i < MAX_RELAYS; i++) {
    _relays[i].state = false;
  }
}

// destructor
Relay16::~Relay16() {
  // free any resources here
  // for example, you can turn off all the relays before destroying the object
  for (int i = 0; i < MAX_RELAYS; i++) {
    digitalWrite(_relays[i].pin, LOW);
  }
}

// initialize the usermod
void Relay16::setup() {
  // loop through the relay pins
  for (int i = 0; i < MAX_RELAYS; i++) {
    // set the pin and state of each relay object
    _relays[i].pin = relayPins[i];
    _relays[i].state = false;
    // set the pin mode as output
    pinMode(relayPins[i], OUTPUT);
  }
}

// update the usermod
void Relay16::loop() {
  // you can write your logic here to control the relays based on some conditions
  // for example, you can toggle a relay every second using millis()
  
  // get the current time in milliseconds
  unsigned long currentTime = millis();
  
  // check if one second has passed since the last time the relay was toggled
  if (currentTime - lastTime > toggleInterval) {
    // update the last time to the current time
    lastTime = currentTime;
    
    // toggle the current relay
    toggleRelay(currentRelay);
    
    // increment the current relay index and wrap around if it reaches the maximum number of relays
    currentRelay = (currentRelay + 1) % MAX_RELAYS;
  }
}

// toggle a relay
void Relay16::toggleRelay(uint8_t index) {
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

// handle button press
bool Relay16::handleButton(uint8_t buttonIdx) {
  // you can write your logic here to handle button presses and control the relays accordingly
  // for example, you can use buttonIdx as an index to toggle a specific relay
  
  // check if the button index is valid
  if (buttonIdx >= 0 && buttonIdx < MAX_RELAYS) {
    // toggle the relay at the button index
    toggleRelay(buttonIdx);
    
    // return true to indicate that the button press was handled by this usermod
    return true;
    
  } else {
    // return false to indicate that the button press was not handled by this usermod and should be passed to other usermods or WLED core functions
    return false;
  }
}

// add to JSON state
void Relay16::addToJsonState(JsonObject &root) {
  // you can write your logic here to add any information about your usermod to the JSON state object that is sent to clients or saved in presets
  // for example, you can add an array of relay states
  
  // create an array object under "umod" key in root object
  JsonArray arr = root["umod"].createNestedArray();
  
  // loop through the relays and add their states to the array object as booleans
  for (int i = 0; i < MAX_RELAYS; i++) {
    arr.add(getRelayState(i));
  }
}

// read from JSON state
void Relay16::readFromJsonState(JsonObject &root) {
   // you can write your logic here to read any information about your usermod from the JSON state object that is received from clients or loaded from presets 
   // for example, you can read an array of relay states
   
   // check if root object has "umod" key and it is an array object 
   if (root.containsKey("umod") && root["umod"].is<JsonArray>()) {
     // get a reference to the array object under "umod" key in root object 
     JsonArray arr = root["umod"].as<JsonArray>();
     
     // loop through the array object and set the relay states accordingly 
     for (int i = 0; i < MAX_RELAYS; i++) {
       // check if the array object has a boolean value at the current index 
       if (arr[i].is<bool>()) {
         // get the boolean value from the array object at the current index 
         bool state = arr[i].as<bool>();
         // set the relay state at the current index to the boolean value 
         setRelayState(i, state);
       }
     }
   }
}

// add to JSON info
void Relay16::addToJsonInfo(JsonObject &root) {
  // you can write your logic here to add any information about your usermod to the JSON info object that is sent to clients
  // for example, you can add a string with the name and version of your usermod
  
  // create a string object under "umod" key in root object
  root["umod"] = F("Relay16 v1.0");
}

// add to config
void Relay16::addToConfig(JsonObject &root) {
  // you can write your logic here to add any configuration options for your usermod to the JSON config object that is saved in the file system
  // for example, you can add an array of relay pins
  
  // create an array object under "umod" key in root object
  JsonArray arr = root["umod"].createNestedArray();
  
  // loop through the relay pins and add them to the array object as integers
  for (int i = 0; i < MAX_RELAYS; i++) {
    arr.add(getRelayPin(i));
  }
}

// read from config
bool Relay16::readFromConfig(JsonObject &root) {
   // you can write your logic here to read any configuration options for your usermod from the JSON config object that is loaded from the file system 
   // for example, you can read an array of relay pins
   
   // check if root object has "umod" key and it is an array object 
   if (root.containsKey("umod") && root["umod"].is<JsonArray>()) {
     // get a reference to the array object under "umod" key in root object 
     JsonArray arr = root["umod"].as<JsonArray>();
     
     // loop through the array object and set the relay pins accordingly 
     for (int i = 0; i < MAX_RELAYS; i++) {
       // check if the array object has an integer value at the current index 
       if (arr[i].is<int>()) {
         // get the integer value from the array object at the current index 
         int pin = arr[i].as<int>();
         // set the relay pin at the current index to the integer value 
         setRelayPin(i, pin);
       }
     }
     
     // return true to indicate that the config was successfully read by this usermod
     return true;
     
   } else {
     // return false to indicate that the config was not read by this usermod and should be passed to other usermods or WLED core functions
     return false;
   }
}
