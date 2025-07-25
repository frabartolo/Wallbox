#include "webserver.h"

// Simple map function (like Arduino's)
static int map_value(int x, int in_min, int in_max, int out_min, int out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

WebServer server(80);

// Generates the HTML page for the web interface
String htmlPage(int amps, ModeState mode) {
  String modeStr = (mode == ModeState::AUTO ? "Auto" : (mode == ModeState::MAX ? "Maximal" : "Excess"));
  return "<html><body><h1>Wallbox Control</h1>"
         "<p>Current: " + String(amps) + " A</p>"
         "<p>Mode: " + modeStr + "</p>"
         "<form action='/set?mode=max' method='POST'><button>Max charge</button></form>"
         "<form action='/set?mode=excess' method='POST'><button>Excess charge</button></form>"
         "<form action='/set?mode=auto' method='POST'><button>Automatic</button></form>"
         "</body></html>";
}

// Sets up the web server and routes
void setupWebServer(DS3502 &poti, ModeState* mode) {
  server.on("/", HTTP_GET, [&]() {
    int amps = map_value(poti.getCurrentSetting(), 127, 0, 8, 64);
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
