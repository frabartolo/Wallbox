/*
   Copyright (c) 2015, Majenko Technologies
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

 * * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

 * * Redistributions in binary form must reproduce the above copyright notice, this
     list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.

 * * Neither the name of Majenko Technologies nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Ticker.h>  //Ticker Library

#ifndef STASSID
#define STASSID "Jungle_tief"
#define STAPSK  "51379055442083999816"
#endif
#define LED 2           //Pin 2 für LED
#define kHzOut 14       //Pin 2 für LED
#define RelaisLinks 0   //Pin 0 für Relais Links
#define RelaisRechts 4  //Pin 4 für Relais Links

const char *ssid = STASSID;
const char *password = STAPSK;
Ticker blinker;

ESP8266WebServer server(80);

//=== Callback Funktionen =================================================
void changeState()
{
  digitalWrite(kHzOut, !(digitalRead(kHzOut)));  //Puls invertieren
  //Serial.println ("GPIO umgeschaltet");
}

void pulseOn()
{
  digitalWrite(kHzOut, LOW);  //Puls einschalten
}

void pulseOff()
{
  digitalWrite(kHzOut, HIGH);  //Puls ausschalten
}

void handleRoot() {
  char temp[800];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, 800,
           "<html>\
              <head>\
                <meta http-equiv='refresh' content='5'/>\
                <title>Wilhelms Wallbox</title>\
                <style>\
                  body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
                </style>\
              </head>\
              <body>\
                <h1>Wilhelms Wallbox</h1>\
                <p>Ich bin irgendwann Wilhelms' Wallbox!</p>\
                <p>Uptime: %02d:%02d:%02d</p>\
                <img src=\"/test.svg\" />\
              </body>\
            </html>",
           hr, min % 60, sec %60
          );
  //Serial.println (strlen(temp));
  server.send(200, "text/html", temp);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}

void drawGraph() {
  String out;
  out.reserve(2600);
  char temp[70];
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
  out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
  out += "<g stroke=\"black\">\n";
  int y = rand() % 130;
  for (int x = 10; x < 390; x += 10) {
    int y2 = rand() % 130;
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
    out += temp;
    y = y2;
  }
  out += "</g>\n</svg>\n";

  server.send(200, "image/svg+xml", out);
}

//=======================================================================
//                               Setup
//=======================================================================
void setup(void) {
  pinMode(RelaisLinks, OUTPUT);
  digitalWrite(RelaisLinks, HIGH);
  pinMode(RelaisRechts, OUTPUT);
  digitalWrite(RelaisRechts, HIGH);
  pinMode(kHzOut, OUTPUT);
  digitalWrite(kHzOut, HIGH);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  //Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  //Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }

  //Serial.println("");
  //Serial.print("Connected to ");
  //Serial.println(ssid);
  //Serial.print("IP address: ");
  //Serial.println(WiFi.localIP());

  //=== erstmal ein Blinken von 0.5 Sekunden ============================
  //blinker.attach(0.5, changeState); 

  //=== hier ein Blinken von 0.5 Millisekunden = 1kHz ===================
  blinker.attach_ms(1, changeState); 
  
  if (MDNS.begin("esp8266")) {
    //Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/test.svg", drawGraph);

  server.on("/linksAn", []() {
    //Einschalten = LOW
    digitalWrite(RelaisLinks, LOW);
    digitalWrite(LED, LOW);
    server.send(200, "text/plain", "links An");
  });
  server.on("/linksAus", []() {
    //Ausschalten = HIGH
    digitalWrite(RelaisLinks, HIGH);    
    digitalWrite(LED, HIGH);
    server.send(200, "text/plain", "links Aus");
  });
  server.on("/rechtsAn", []() {
    //Einschalten = LOW
    digitalWrite(RelaisRechts, LOW);    
    digitalWrite(LED, LOW);
    server.send(200, "text/plain", "Rechts An");
  });
  server.on("/rechtsAus", []() {
    //Ausschalten = HIGH
    digitalWrite(RelaisRechts, HIGH);
    digitalWrite(LED, HIGH);
    server.send(200, "text/plain", "Rechts Aus");
  });

  
  server.on("/an", []() {
    char temp[600];
    int sec = millis() / 1000;
    int min = sec / 60;
    int hr = min / 60;
  
    snprintf(temp, 600,
             "<html>\
                <head>\
                  <meta http-equiv='refresh' content='5'/>\
                  <title>Wilhelms Wallbox</title>\
                  <style>\
                    body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
                  </style>\
                </head>\
                <body>\
                  <h1>Wilhelms Wallbox</h1>\
                  <p>Uptime: %02d:%02d:%02d</p>\
                  <p>Port eingeschaltet!</p>\
                </body>\
              </html>",
             hr, min % 60, sec %60
            );
    server.send(200, "text/html", temp);
    //Serial.println (strlen(temp));
    //server.send(200, "text/plain", "Port eingeschaltet");
    //server.send(200, "text/html", getSchalttext("kHzOut eingeschaltet"));
    //Serial.println(getSchalttext("Port eingeschaltet"));
    digitalWrite(RelaisLinks, LOW);
    digitalWrite(RelaisRechts, HIGH);
    digitalWrite(LED, LOW);
  });

  server.on("/aus", []() {
    char temp[600];
    int sec = millis() / 1000;
    int min = sec / 60;
    int hr = min / 60;
  
    snprintf(temp, 600,
             "<html>\
                <head>\
                  <meta http-equiv='refresh' content='5'/>\
                  <title>Wilhelms Wallbox</title>\
                  <style>\
                    body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
                  </style>\
                </head>\
                <body>\
                  <h1>Wilhelms Wallbox</h1>\
                  <p>Uptime: %02d:%02d:%02d</p>\
                  <p><B>Port ausgeschaltet!</B></p>\
                </body>\
              </html>",
             hr, min % 60, sec %60
            );
    server.send(200, "text/html", temp);
    //Serial.println (strlen(temp));
    //server.send(200, "text/plain", "Port ausgeschaltet");
    //server.send(200, "text/html", getSchalttext("Port ausgeschaltet"));
    //Serial.println(getSchalttext("Port ausgeschaltet"));
    digitalWrite(RelaisLinks, HIGH);
    digitalWrite(RelaisRechts, LOW);
    digitalWrite(LED, HIGH);
  });

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);
  
  server.begin();
  
  //Serial.println("HTTP server started");
};

String getSchalttext (String text){
  String neuerText = 
  "<html>\
        <head>\
          <meta http-equiv='refresh' content='5'/>\
          <title>Wilhelms Wallbox</title>\
          <style>\
            body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
          </style>\
        </head>\
        <body>\
          <h1>Wilhelms Wallbox</h1>\
          <p>Port eingeschaltet!";
    //Serial.println (neuerText.length());
    neuerText = String (neuerText + text);
    neuerText = String (neuerText + " geschaltet!</p>\
        </body>\
      </html>");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}
