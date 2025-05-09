#pragma once

#include <Arduino.h>

String ISODateUtc(
    uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
String ISODateUtcMillis(
    uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, uint16_t millisecond = 0);