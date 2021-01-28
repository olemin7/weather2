/*
 * CConfig.cpp
 *
 *  Created on: Jan 16, 2021
 *      Author: ominenko
 */

#include "CConfig.h"
#include "libs/logs.h"
#include <LittleFS.h>
#include <vector>
using namespace std;

CConfig config;

bool CConfig::setup() {
    DBG_FUNK();
    auto cmdFile = LittleFS.open(JSON_FILE_CONFIG, "r");
    DeserializationError error = deserializeJson(json_config, cmdFile);
    if (error) {
        DBG_OUT << "Failed to read file, using default configuration" << endl;
        return false;
    }
    bool isOk = true;
    const auto keys = vector<string> {
            "DEVICE_NAME",
            "MQTT_SERVER", "MQTT_PORT", "MQTT_PERIOD",
            "OTA_USERNAME", "OTA_PASSWORD" };
    for (const auto &key : keys) {
        if (!json_config.containsKey(key.c_str())) {
            isOk = false;
            DBG_OUT << "miss keys:" << key << endl;
        }
    }
    if (!isOk) {
        return false;
    }
    serializeJsonPretty(json_config, Serial);
    DBG_OUT << endl;
    DBG_OUT << "config " << json_config.capacity() << ":" << json_config.memoryUsage() << endl;
    return true;
}
void CConfig::setDefault() {
    json_config.clear();
    //---------------------------------
    json_config["DEVICE_NAME"] = DEF_DEVICE_NAME;
    json_config["MQTT_SERVER"] = "";
    json_config["MQTT_PORT"] = 0;
    json_config["MQTT_PERIOD"] = 60 * 1000;
    json_config["OTA_USERNAME"] = "";
    json_config["OTA_PASSWORD"] = "";
//-----------------------------------------------
    DBG_OUT << "default config " << json_config.capacity() << ":" << json_config.memoryUsage() << endl;
    serializeJsonPretty(json_config, Serial);
    DBG_OUT << endl;
}

