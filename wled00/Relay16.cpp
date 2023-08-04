// Relay16.cpp
// Relay16 usermod for WLED

#include <Relay16.h>
#include <ESPAsyncWebServer.h>


#include <LittleFS.h> 

// Constructor
Relay16::Relay16() {
  // initialize any variables or resources here
  // for example, you can set the default state of the relays to off
  for (uint8_t i = 0; i < MAX_RELAYS; i++) {
    _relays[i].state = false;
  }
}

// Destructor
Relay16::~Relay16() {
  // free any resources here
  // for example, you can turn off all the relays before destroying the object
  for (uint8_t i = 0; i < MAX_RELAYS; i++) {
    digitalWrite(_relays[i].pin, LOW);
  }
}
void Relay16::renameSwitch(AsyncWebServerRequest &request) {
  if (request.hasParam("device", true)) { // Check if the request has a parameter named "device" in the body
    int switchIndex = request.getParam("device", true)->value().toInt(); // Get the value of the parameter as an integer
    String newName = request.getParam("name", true)->value(); // Get the value of another parameter named "name" as a string
    if (switchIndex >= 0 && switchIndex < MAX_RELAYS) {
      _relays[switchIndex].name = newName;
    }
  }
  request.send(200, "text/plain", "OK");
}


// Generate the HTML page dynamically
void Relay16::addToPage(AsyncWebServerRequest &request) {


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
      request.open('GET', '/win&device=' + relayIndex + '&value=' + (isChecked ? 1 : 0), true);
      request.send();
    }
    
    function renameSwitches(switchIndex) {
      const newName = prompt('Enter the new name for the switch:');
      if (newName !== null) {
        // Send the new name to the server via an AJAX request
        const request = new XMLHttpRequest();
        request.open('GET', '/win&rename=' + switchIndex + '&name=' + encodeURIComponent(newName), true);
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
  request.send(200, "text/html", html);
}

void Relay16::toggleRelay(uint8_t index) {
  if (index < MAX_RELAYS) {
    _relays[index].state = !_relays[index].state;
    digitalWrite(_relays[index].pin, _relays[index].state ? HIGH : LOW);
  }
}

int8_t Relay16::getRelayPin(uint8_t index) {
  if (index < MAX_RELAYS) {
    return _relays[index].pin;
  }
  return -1; // Return an invalid value if the index is out of range
}
bool Relay16::getRelayState(uint8_t index) {
  if (index < MAX_RELAYS) {
    return _relays[index].state;
  }
  return false; // Return false if the index is out of range
}
void Relay16::setRelayPin(uint8_t index, uint8_t pin) {
  if (index < MAX_RELAYS) {
    _relays[index].pin = pin;
  }
}

void Relay16::setRelayState(uint8_t index, bool state) {
  if (index < MAX_RELAYS) {
    _relays[index].state = state;
    digitalWrite(_relays[index].pin, state ? HIGH : LOW);
  }
}

bool Relay16::handleButton(uint8_t buttonIdx) {
  // This function is called when a button is pressed on WLED's web interface.
  // You can implement custom logic here to handle button presses.
  // For example, you can toggle a specific relay when a button is pressed.
  
  // Here's an example of how you can toggle relay 0 when button 0 is pressed:
  if (buttonIdx == 0) {
    toggleRelay(0);
    return true; // Return true to indicate that the button press is handled by this usermod
  }

  // If the button press is not handled by this usermod, return false.
  return false;
}





// Initialize the usermod
void Relay16::setup() {
  

Serial.println("SETUP STARTED");
   Serial.begin(115200);
  if (!LITTLEFS.begin()) { // Attempt to mount LittleFS
    Serial.println("Failed to mount LittleFS filesystem!");
    return;
  }
 // Check the total and used bytes on SPIFFS
  size_t totalBytes = LITTLEFS.totalBytes();
  size_t usedBytes = LITTLEFS.usedBytes();

  Serial.print("Total LITTLEFS space: ");
  Serial.print(totalBytes);
  Serial.println(" bytes");

  Serial.print("Used LITTLEFS space: ");
  Serial.print(usedBytes);
  Serial.println(" bytes");

  Serial.print("Available LITTLEFS space: ");
  Serial.print(totalBytes - usedBytes);
  Serial.println(" bytes");

  // Loop through the relay pins
  
  for (uint8_t i = 0; i < MAX_RELAYS; i++) {
    // Set the pin and state of each relay object
    _relays[i].pin = relayPins[i];
    _relays[i].state = false;
    // Set the pin mode as output
    pinMode(relayPins[i], OUTPUT);
  }


    server.on("/relay_control.html", HTTP_GET, [](AsyncWebServerRequest *request){
      Serial.println("Received request for /relay_control.html");
    if (LITTLEFS.exists("/relay_control.html")) {
    Serial.println("Serving /relay_control.html");
    request->send(LITTLEFS, "/relay_control.html", "text/html");
      } else {
    Serial.println("/relay_control.html not found");
    request->send(404);
    }
    });

 
  // Register the "/win" endpoint to handle switch renaming
  server.on("/win", HTTP_GET, [this](AsyncWebServerRequest *request) {
    if (request->hasArg("rename")) {
      renameSwitch(*request);
    }
  });
}

// Update the usermod
void Relay16::loop() {
  // You can write your logic here to control the relays based on some conditions
  // For example, you can toggle a relay every second using millis()

  // Get the current time in milliseconds
  unsigned long currentTime = millis();

  // Check if one second has passed since the last time the relay was toggled
  if (currentTime - lastTime > toggleInterval) {
    // Update the last time to the current time
    lastTime = currentTime;

    // Toggle the current relay
    toggleRelay(currentRelay);

    // Increment the current relay index and wrap around if it reaches the maximum number of relays
    currentRelay = (currentRelay + 1) % MAX_RELAYS;
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
