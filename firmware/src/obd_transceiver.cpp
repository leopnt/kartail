#include "obd_transceiver.h"
#include "pins.h"
#include "exception.h"

#include "global_logger.h"

OBDTransceiver::OBDTransceiver(/* args */)
{
}

OBDTransceiver::~OBDTransceiver()
{
}

bool OBDTransceiver::Begin()
{
    m_obd.begin(OBD_SERIAL, OBD_RX, OBD_TX);

    if (!m_obd.initKWPSlow())
    {
        return false;
    }

    return true;
}

void OBDTransceiver::Process()
{
    OBDPacket obdPacket;

    // extracted from Dacia Sandero I, profile/Sensor pid in Car Scanner App
    // and https://www.carscanner.info/custompids/
    const uint8_t MODE = 0x22;

    obdPacket.id = m_pidPickList[m_currentPid];

    if (!m_obd.getPID(obdPacket.id, MODE, 4))
    {
        Serial.println("WARNING: getting OBD did not work");
        return;
    }
    delay(10);

    obdPacket.data = m_obd.readUint32();
    Log.Info("OBD " + String(obdPacket.id) + ": 0x" + String(obdPacket.data, 16));

    if (obdPacket.id != m_lastPacket.id || obdPacket.data != m_lastPacket.data)
    {
        m_packetHasChanged = true;
        m_lastPacket = obdPacket;
    }

    // increment for next iteration
    m_currentPid = (m_currentPid + 1) % sizeof(m_pidPickList);
}

bool OBDTransceiver::HasChanged()
{
    if (m_packetHasChanged)
    {
        m_packetHasChanged = false;
        return true;
    }

    return false;
}

uint8_t OBDTransceiver::ID()
{
    return m_lastPacket.id;
}

uint32_t OBDTransceiver::Data()
{
    return m_lastPacket.data;
}
