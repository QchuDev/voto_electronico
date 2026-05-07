#include <Arduino.h>
#include "component/ConnectionManager.h"
#include "component/QRDisplay.h"
#include "component/Scanner.h"

// Components 
ConnectionManager coMan;    // MQTT connection  ->> Sub and Pub
QRDisplay qrDis;            // Display of the QR
Scanner scanner;        // Get data from the scanner

void setup() {
  Serial.begin(9600); delay(10);
  
  coMan.setup();          // Setup MQTT Connection
  qrDis.setup();          // Setup Display
  scanner.setup();          // Setup scanner
}

void loop() {
  coMan.update();         // Update the state of the MQTT manager
  qrDis.update();         // Update the display to draw
  scanner.update();        // Update the scanner 
  delay(5000);
}
