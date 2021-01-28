/*
 * CConfig.h
 *
 *  Created on: Jan 28, 2021
 *      Author: ominenko
 */

#pragma once
#include <sstream>
#include <ArduinoJson.h>

constexpr auto JSON_FILE_CONFIG = "/www/config/config.json";

#define DEF_DEVICE_NAME "weather_dev"
#define DEF_AP_PWD "12345678"

class CConfig {
    StaticJsonDocument<512> json_config;
    public:
    bool setup();
    void setDefault();
    const char* getDeviceName() const {
        return json_config["DEVICE_NAME"].as<const char*>();
    }

    const char* getMqttServer() const {
        return json_config["MQTT_SERVER"].as<const char*>();
    }
    const int getMqttPort() const {
        return json_config["MQTT_PORT"].as<int>();
    }
    const unsigned long getMqttPeriod() const {
        return json_config["MQTT_PERIOD"].as<unsigned long>(); //in ms
    }
    const char* getOtaUsername() const {
        return json_config["OTA_USERNAME"].as<const char*>();
    }
    const char* getOtaPassword() const {
        return json_config["OTA_PASSWORD"].as<const char*>();
    }

};
extern CConfig config;
