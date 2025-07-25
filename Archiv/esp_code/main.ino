#include "ds3502.h"
#include "webserver.h"

DS3502 poti;
ModeState mode = ModeState::AUTO;

constexpr unsigned long OVERRIDE_DURATION = 3600000; // 1 hour in ms
constexpr unsigned long CHECK_INTERVAL = 5000;       // 5 seconds in ms

void setup() {
  Serial.begin(115200);
  WiFi.begin("DEIN_WIFI", "DEIN_PASS");

  unsigned long wifiStart = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (millis() - wifiStart > 20000) { // 20 seconds timeout
      Serial.println("WiFi connection failed!");
      return;
    }
  }

  poti.begin();
  setupWebServer(poti, &mode);

  Serial.println("System bereit");
}

void loop() {
  static unsigned long lastCheck = 0;
  static unsigned long overrideStart = 0;

  // Switch back to AUTO mode after override duration
  if (mode == ModeState::MAX && millis() - overrideStart > OVERRIDE_DURATION) {
    mode = ModeState::AUTO;
    poti.setWiperForAmps(12);
  }

  // Periodic check
  if (millis() - lastCheck > CHECK_INTERVAL) {
    lastCheck = millis();
    if (mode == ModeState::AUTO) {
      float pvPower = getMockedPVPower();
      if (pvPower < 300) {
        poti.setWiperForAmps(32);
      } else {
        poti.setWiperForAmps(12);
      }
    }
  }
}
