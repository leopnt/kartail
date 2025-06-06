#include "gps.h"
#include "pins.h"

#include "logger.h"

GPS::GPS()
{
    m_gpsSerial = &GPS_SERIAL;
    m_gpsSerial->begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

    m_nmeaParser = TinyGPSPlus();

    m_lastSecond = 0;
    m_timeHasChanged = false;

    m_lastLat = 0;
    m_lastLon = 0;
    m_locationHasChanged = false;
}

GPS::~GPS()
{
    m_gpsSerial = 0;
}

void GPS::Process()
{
    while (m_gpsSerial->available())
        m_nmeaParser.encode(m_gpsSerial->read());

    bool isTimeValid = m_nmeaParser.date.isValid() && m_nmeaParser.time.isValid();
    if (m_lastSecond != m_nmeaParser.time.second() && isTimeValid)
    {
        m_lastSecond = m_nmeaParser.time.second();
        m_timeHasChanged = true;
    }

    bool isLocationValid = m_nmeaParser.location.isValid();
    if ((m_lastLat != m_nmeaParser.location.lat() || m_lastLon != m_nmeaParser.location.lng()) && isLocationValid)
    {
        m_lastLat = m_nmeaParser.location.lat();
        m_lastLon = m_nmeaParser.location.lng();
        m_locationHasChanged = true;
    }
}

bool GPS::TimeHasChanged()
{
    if (m_timeHasChanged)
    {
        m_timeHasChanged = false;
        return true;
    }

    return false;
}

bool GPS::LocationHasChanged()
{
    if (m_locationHasChanged)
    {
        m_locationHasChanged = false;
        return true;
    }

    return false;
}

bool GPS::HasChanged()
{
    return TimeHasChanged() && LocationHasChanged();
}

bool GPS::IsValid()
{
    bool satellites = m_nmeaParser.satellites.isValid();
    bool hdop = m_nmeaParser.hdop.isValid();
    bool date = m_nmeaParser.date.isValid();
    bool time = m_nmeaParser.time.isValid();
    bool location = m_nmeaParser.location.isValid();
    bool altitude = m_nmeaParser.altitude.isValid();

    return date && time && location && altitude;
}

bool GPS::IsReadyForTimeSync()
{
    return m_nmeaParser.location.age() < 500;
}

uint16_t GPS::Year()
{
    return m_nmeaParser.date.year();
}

uint8_t GPS::Month()
{
    return m_nmeaParser.date.month();
}

uint8_t GPS::Day()
{
    return m_nmeaParser.date.day();
}

uint8_t GPS::Hour()
{
    return m_nmeaParser.time.hour();
}

uint8_t GPS::Minute()
{
    return m_nmeaParser.time.minute();
}

uint8_t GPS::Second()
{
    return m_nmeaParser.time.second();
}

double GPS::Lat()
{
    return m_nmeaParser.location.lat();
}

double GPS::Lng()
{
    return m_nmeaParser.location.lng();
}

double GPS::Ele()
{
    return m_nmeaParser.altitude.meters();
}

uint32_t GPS::NumSatellites()
{
    return m_nmeaParser.satellites.value();
}

double GPS::Hdop()
{
    return m_nmeaParser.hdop.hdop();
}

String GPS::ToString()
{
    String out = String("");

    out += String(m_nmeaParser.location.lat(), 6);
    out += String(",");
    out += String(m_nmeaParser.location.lng(), 6);
    out += String(",");
    out += String(m_nmeaParser.altitude.meters(), 1);

    return out;
}
