#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <WiFi.h>
#include <WebServer.h>
#include "ds3502.h"

// Operation mode for the wallbox
enum class ModeState { AUTO, MAX, EXCESS };

// Sets up the web server and routes
void setupWebServer(DS3502 &poti, ModeState* mode);

#endif
