/*
 * misk.h
 *
 *  upd 30 dec-2020
 *      Author: ominenko
 */

#pragma once
#include <Arduino.h>
#include <stdint.h>
#include <string>
#include <ostream>

std::ostream& operator<<(std::ostream &os, const String &str);
//set nan as null
void toJson(std::ostream &os, const float &var);

void LED_ON();
void LED_OFF();
void blink();

String getMimeType(String path);
void hw_info(std::ostream &out);
void LittleFS_info(std::ostream &out);

bool isExtMach(const std::string &name, const std::string &ext);

struct CompareCStrings
{
    bool operator()(const char *lhs, const char *rhs) const
            {
        return strcmp(lhs, rhs) < 0;
    }
};
std::string getResetInfo();
std::string to_string(uint32_t ul);

/*
 *  on start 0 during timeout 1
 */
bool isSafeMode(const uint8_t pin, unsigned long timeout);
