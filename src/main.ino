#include "ds3502.h"
#include "webserver.h"

DS3502 poti;
ModeState mode = ModeState::AUTO;

void setup() {
  Serial.begin(115200);
  WiFi.begin("DEIN_WIFI", "DEIN_PASS");

  while (WiFi.status() != WL_CONNECTED) delay(500);

  poti.begin();
  setupWebServer(poti, &mode);

  Serial.println("System bereit");
}

void loop() {
  static unsigned long lastCheck = 0;
  static unsigned long overrideStart = 0;

  if (mode == ModeState::MAX && millis() - overrideStart > 3600000) {
    mode = ModeState::AUTO;
    poti.setWiperForAmps(12);
  }

  if (millis() - lastCheck > 5000) {
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
