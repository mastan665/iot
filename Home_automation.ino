// ==========================================================
// ESP32 4-Relay IoT Home Automation with Feedback (Fixed)
// Author: Aenugu Mastanaiah
// ==========================================================

#define BLYNK_TEMPLATE_ID "TMPL3sMm5ECz7"
#define BLYNK_TEMPLATE_NAME "ESP32 Home Control"
#define BLYNK_AUTH_TOKEN "I1z7PFDYUIiYqnSjN3WiPa06vsFyJWhZ"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Mastan";
char pass[] = "msnp3888";

// ---------------- Pin Assignments ----------------
int relayPins[4]  = {26, 27, 14, 12};   // Relay control pins
int switchPins[4] = {32, 33, 25, 4};   // Manual switch inputs

// Relay states (HIGH = OFF, LOW = ON)
int relayState[4] = {HIGH, HIGH, HIGH, HIGH};

// ------------- Helper Function -------------
void setRelay(uint8_t index, int state) {
  digitalWrite(relayPins[index], state);
}

// ------------- Blynk Virtual Write Handlers -------------
BLYNK_WRITE(V0) { relayState[0] = param.asInt() ? LOW : HIGH; setRelay(0, relayState[0]); }
BLYNK_WRITE(V1) { relayState[1] = param.asInt() ? LOW : HIGH; setRelay(1, relayState[1]); }
BLYNK_WRITE(V2) { relayState[2] = param.asInt() ? LOW : HIGH; setRelay(2, relayState[2]); }
BLYNK_WRITE(V3) { relayState[3] = param.asInt() ? LOW : HIGH; setRelay(3, relayState[3]); }

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Initialize relay pins
  for (int i = 0; i < 4; i++) {
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], HIGH);  // Start OFF
  }

  // Initialize switch pins
  for (int i = 0; i < 4; i++) {
    pinMode(switchPins[i], INPUT_PULLUP);  // active LOW
  }

  Serial.println("✅ ESP32 4-Relay Control with Manual Feedback Ready");
}

void loop() {
  Blynk.run();

  // Read manual switch inputs
  for (int i = 0; i < 4; i++) {
    int swState = digitalRead(switchPins[i]);
    if (swState == LOW) {                // switch pressed
      relayState[i] = !relayState[i];    // toggle state
      setRelay(i, relayState[i]);        // update relay
      Blynk.virtualWrite(V0 + i, relayState[i] == LOW ? 1 : 0); // sync app
      delay(300); // debounce
    }
  }
}
