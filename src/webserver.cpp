#include "webserver.h"

WebServer server(80);

String htmlPage(int amps, ModeState mode) {
  String modeStr = (mode == ModeState::AUTO ? "Auto" : (mode == ModeState::MAX ? "Maximal" : "Überschuss"));
  return "<html><body><h1>Wallbox Steuerung</h1>"
         "<p>Aktueller Strom: " + String(amps) + " A</p>"
         "<p>Modus: " + modeStr + "</p>"
         "<form action='/set?mode=max' method='POST'><button>Maximal laden</button></form>"
         "<form action='/set?mode=excess' method='POST'><button>Überschussladen</button></form>"
         "<form action='/set?mode=auto' method='POST'><button>Automatik</button></form>"
         "</body></html>";
}

void setupWebServer(DS3502 &poti, ModeState* mode) {
  server.on("/", HTTP_GET, [&]() {
    int amps = map(poti.getCurrentSetting(), 127, 0, 8, 64);
    server.send(200, "text/html", htmlPage(amps, *mode));
  });

  server.on("/set", HTTP_POST, [&]() {
    if (server.hasArg("mode")) {
      String m = server.arg("mode");
      if (m == "max") {
        *mode = ModeState::MAX;
        poti.setWiperForAmps(32);
      } else if (m == "excess") {
        *mode = ModeState::EXCESS;
        poti.setWiperForAmps(12);
      } else {
        *mode = ModeState::AUTO;
      }
    }
    server.sendHeader("Location", "/");
    server.send(303);
  });

  server.begin();
}
