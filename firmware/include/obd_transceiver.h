#pragma once

#include <Arduino.h>
#include "pins.h"
#include "OBD9141.h"

struct OBDPacket
{
    uint8_t id;
    uint32_t data;
};

class OBDTransceiver
{
private:
    OBD9141 m_obd;
    OBDPacket m_lastPacket;
    bool m_packetHasChanged;

    uint8_t m_pidPickList[3] = {0x02, 0x03, 0x05}; // manifold pressure, water temperature, engine speed
    uint8_t m_currentPid = 0;

public:
    OBDTransceiver(/* args */);
    ~OBDTransceiver();

    bool Begin();
    void Process();
    bool HasChanged();

    uint8_t ID();
    uint32_t Data();
};
