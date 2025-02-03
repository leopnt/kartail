#include "clock.h"

#include "TimeLib.h"

Clock::Clock() : m_lastMillis(0)
{
}

Clock::~Clock()
{
}

void Clock::Sync(GPS &gps)
{
    setTime(gps.Hour(), gps.Minute(), gps.Second(), gps.Day(), gps.Month(), gps.Year());
    m_lastMillis = millis();
}

uint16_t Clock::Year() const
{
    return year();
}

uint8_t Clock::Month() const
{
    return month();
}

uint8_t Clock::Day() const
{
    return day();
}

uint8_t Clock::Hour() const
{
    return hour();
}

uint8_t Clock::Minute() const
{
    return minute();
}

uint8_t Clock::Second() const
{
    return second();
}

uint16_t Clock::Millis() const
{
    uint32_t delta = millis() - m_lastMillis;
    return delta % 1000;
}