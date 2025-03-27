#pragma once

#include <Arduino.h>
#include <TinyGPSPlus.h>

class GPS
{
private:
    HardwareSerial *m_gpsSerial = NULL;
    TinyGPSPlus m_nmeaParser;

    uint8_t m_lastSecond;
    bool m_timeHasChanged;

    double m_lastLat;
    double m_lastLon;
    bool m_locationHasChanged;

public:
    GPS();
    ~GPS();

    void Process();
    bool TimeHasChanged();
    bool LocationHasChanged();
    bool HasChanged();
    bool IsValid();
    bool IsFirstFixAcquired();
    bool IsReadyForTimeSync();
    uint16_t Year();
    uint8_t Month();
    uint8_t Day();
    uint8_t Hour();
    uint8_t Minute();
    uint8_t Second();
    double Lat();
    double Lng();
    double Ele();
    uint32_t NumSatellites();
    double Hdop();
    String ToString();
};
