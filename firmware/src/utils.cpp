#include "utils.h"

String ISODateUtc(const Clock &clock)
{
    return ISODateUtc(
        clock.Year(), clock.Month(), clock.Day(), clock.Hour(), clock.Minute(), clock.Second());
}

String ISODateUtcMillis(const Clock &clock)
{
    return ISODateUtcMillis(
        clock.Year(), clock.Month(), clock.Day(), clock.Hour(), clock.Minute(), clock.Second(), clock.Millis());
}

String ISODateUtc(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second)
{
    String out = String("");

    out += String(year);
    out += String("-");
    out += String(month < 10 ? "0" : "") + String(month); // Zero-padding month
    out += String("-");
    out += String(day < 10 ? "0" : "") + String(day);
    out += String("T");
    out += String(hour < 10 ? "0" : "") + String(hour);
    out += String(":");
    out += String(minute < 10 ? "0" : "") + String(minute);
    out += String(":");
    out += String(second < 10 ? "0" : "") + String(second);
    out += String("Z");

    return out;
}

String ISODateUtcMillis(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, uint16_t millisecond)
{
    String out = String("");

    out += String(year);
    out += String("-");
    out += String(month < 10 ? "0" : "") + String(month); // Zero-padding month
    out += String("-");
    out += String(day < 10 ? "0" : "") + String(day);
    out += String("T");
    out += String(hour < 10 ? "0" : "") + String(hour);
    out += String(":");
    out += String(minute < 10 ? "0" : "") + String(minute);
    out += String(":");
    out += String(second < 10 ? "0" : "") + String(second);
    out += String(".");
    if (millisecond < 10)
        out += "00";
    else if (millisecond < 100)
        out += "0";
    out += String(millisecond);
    out += String("Z");

    return out;
}
