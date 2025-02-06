#pragma once

#include <Arduino.h>
#include "gps.h"

class Clock
{
private:
    uint32_t m_lastMillis;

public:
    Clock();
    ~Clock();
    void Sync(GPS &gps);
    uint16_t Year() const;
    uint8_t Month() const;
    uint8_t Day() const;
    uint8_t Hour() const;
    uint8_t Minute() const;
    uint8_t Second() const;
    uint16_t Millis() const;
};
