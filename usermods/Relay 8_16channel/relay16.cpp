// Relay16.cpp
// Relay16 usermod for WLED
#include "relay_control.html"
#include "Relay16.h"
#include <ESPAsyncWebServer.h>
// constructor
Relay16::Relay16() {
  // initialize any variables or resources here
  // for example, you can set the default state of the relays to off
  for (uint8_t i = 0; i < MAX_RELAYS; i++) {
    _relays[i].state = false;
  }
}

// destructor
Relay16::~Relay16() {
  // free any resources here
  // for example, you can turn off all the relays before destroying the object
  for (uint8_t i = 0; i < MAX_RELAYS; i++) {
    digitalWrite(_relays[i].pin, LOW);
  }
}

// initialize the usermod
void Relay16::setup() {
  // loop through the relay pins
  for (uint8_t i = 0; i < MAX_RELAYS; i++) {
    // set the pin and state of each relay object
    _relays[i].pin = relayPins[i];
    _relays[i].state = false;
    // set the pin mode as output
    pinMode(relayPins[i], OUTPUT);
  }
  // Add the "rename" endpoint to handle switch renaming
  server->on("/win", HTTP_GET, [this]() {
    if (server->hasArg("rename")) {
      renameSwitch(server);
    }
  });
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

void Relay16::renameSwitch(AsyncWebServer* server) {
  if (server->hasParam("device", true)) { // check if the request has a parameter named "device" in the body
    int switchIndex = server->getParam("device", true)->value().toInt(); // get the value of the parameter as an integer
    String newName = server->getParam("name", true)->value(); // get the value of another parameter named "name" as a string
    if (switchIndex >= 0 && switchIndex < MAX_RELAYS) {
      _relays[switchIndex].name = newName;
    }
  }
  server->send(200, "text/plain", "OK");
}



void Relay16::addToPage(AsyncWebServer* server) {
  server->sendHeader("Access-Control-Allow-Origin", "*");
  server->send(200, "text/html", html);

  
  String html = R"(
    <!DOCTYPE html>
    <html>
    <head>
      <title>Relay Control</title>
      <style>
    body {
      background-color: #222;
      color: #fff;
    }
    
    .switch-label {
      color: #fff;
    }
    
    .switch input:checked + .slider {
      background-color: #4CAF50;
    }
    
    .slider {
      background-color: #ccc;
    }
  </style>
      <script>
        
    function toggleSwitch(switchElement) {
      const switchId = switchElement.id;
      const isChecked = switchElement.checked;
      const relayIndex = parseInt(switchId.replace('relay_', ''));
      
      // Make an AJAX request to control the relay state
      const request = new XMLHttpRequest();
      request.open('GET', '/win&relay=' + relayIndex + '&value=' + (isChecked ? 1 : 0), true);
      request.send();
    }
    
    function renameSwitch(switchIndex) {
      const newName = prompt('Enter the new name for the switch:');
      if (newName !== null) {
        // Send the new name to the server via an AJAX request
        const request = new XMLHttpRequest();
        request.open('GET', '/win&rename=' + switchIndex + '&value=' + encodeURIComponent(newName), true);
        request.send();
        
        // Update the switch label on the webpage
        const switchLabelElement = document.getElementById('switch_label_' + switchIndex);
        if (switchLabelElement) {
          switchLabelElement.innerHTML = newName;
        }
      }
    }
  </script>

        
      
    </head>
    <body>
      <h1>Relay Control</h1>
  )";

  for (int i = 0; i < MAX_RELAYS; i++) {
    String switchLabel = "Relay " + String(i + 1); // Default switch label
    html += "<div class=\"switch-label\">" + switchLabel + "</div>";
    html += "<label class=\"switch\">";
    html += "<input type=\"checkbox\" id=\"relay_" + String(i) + "\" onclick=\"toggleSwitch(this)\">";
    html += "<span class=\"slider\"></span>";
    html += "</label>";
  }

  html += R"(
    </body>
    </html>
  )";
  
  server->send(200, "text/html", html);
}

}


// toggle a relay
void Relay16::toggleRelay(uint8_t index) {
  // check if the index is valid
  if (index < MAX_RELAYS) {
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
  if (buttonIdx < MAX_RELAYS) {
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
void Relay16::addToJsonState(JsonObject& root) {
  // Add the current relay states to the JSON state
  JsonArray arr = root.createNestedArray("relays");
  for (int i = 0; i < MAX_RELAYS; i++) {
    arr.add(getRelayState(i));
  }
  
  // Add the current switch names to the JSON state
  JsonArray namesArr = root.createNestedArray("relayNames");
  for (int i = 0; i < MAX_RELAYS; i++) {
    namesArr.add(_relays[i].name);
  }
}


// read from JSON state
void Relay16::readFromJsonState(JsonObject& root) {
  // Read the relay states from the JSON state
  if (root.containsKey("relays") && root["relays"].is<JsonArray>()) {
    JsonArray arr = root["relays"].as<JsonArray>();
    for (int i = 0; i < MAX_RELAYS; i++) {
      if (arr[i].is<bool>()) {
        bool state = arr[i].as<bool>();
        setRelayState(i, state);
      }
    }
  }
  
  // Read the relay names from the JSON state
  if (root.containsKey("relayNames") && root["relayNames"].is<JsonArray>()) {
    JsonArray namesArr = root["relayNames"].as<JsonArray>();
    for (int i = 0; i < MAX_RELAYS; i++) {
      if (namesArr[i].is<String>()) {
        String name = namesArr[i].as<String>();
        _relays[i].name = name;
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
  for (uint8_t i = 0; i < MAX_RELAYS; i++) {
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
    for (uint8_t i = 0; i < MAX_RELAYS; i++) {
      // check if the array object has an integer value at the current index
      if (arr[i].is<int>()) {
        // get the integer value from the array object at the current index
        uint8_t pin = arr[i].as<int>();
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
