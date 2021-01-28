/*
 * clock.h
 *
 *  Created on: 23 ????. 2017 ?.
 *      Author: User
 */

#ifndef CLOCK_CLOCK_H_
#define CLOCK_CLOCK_H_

#include <Arduino.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <LittleFS.h>

#include <Wire.h> // must be included here so that Arduino library object file references work
#include <pgmspace.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <ctime>

#include "DHTesp.h"

#include "./libs/CMQTT.h"
#include "./libs/misk.h"
#include "./libs/wifiHandle.h"

#include "./libs/logs.h"
#include <CConfig.h>

#define DEVICE_VERSION DEF_DEVICE_NAME __DATE__  " " __TIME__

#define SERIAL_BAUND 115200
#define SERVER_PORT_WEB 80
#define WIFI_CONNECT_TIMEOUT 20000 //ms
#endif /* CLOCK_CLOCK_H_ */
