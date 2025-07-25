#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <WiFi.h>
#include <WebServer.h>
#include "ds3502.h"

enum class ModeState { AUTO, MAX, EXCESS };

void setupWebServer(DS3502 &poti, ModeState* mode);

#endif
