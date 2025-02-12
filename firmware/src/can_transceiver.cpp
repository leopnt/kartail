#include <CAN.h>
#include "can_transceiver.h"
#include "pins.h"
#include "exception.h"

CANTransceiver::CANTransceiver(/* args */)
{
}

CANTransceiver::~CANTransceiver()
{
}

bool CANTransceiver::Begin()
{
    CAN.setPins(CAN_RX, CAN_TX);
    if (!CAN.begin(500e3))
    {
        return false;
    }

    return true;
}

void CANTransceiver::Process()
{
    if (CAN.parsePacket())
    {
        CANPacket packet;
        packet.id = CAN.packetId();
        packet.isExtended = CAN.packetExtended();
        packet.isRtr = CAN.packetRtr();
        packet.data = String();

        while (CAN.available())
        {
            packet.data += (char)CAN.read();
        }

        m_lastPacket = packet;
        m_packetHasChanged = true;
    }
}

bool CANTransceiver::HasChanged()
{
    if (m_packetHasChanged)
    {
        m_packetHasChanged = false;
        return true;
    }

    return false;
}

long CANTransceiver::ID()
{
    return m_lastPacket.id;
}

bool CANTransceiver::IsExtended()
{
    return m_lastPacket.isExtended;
}

bool CANTransceiver::IsRtr()
{
    return m_lastPacket.isRtr;
}

String CANTransceiver::Data()
{
    return m_lastPacket.data;
}
