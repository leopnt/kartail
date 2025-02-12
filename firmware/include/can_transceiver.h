#pragma once

struct CANPacket
{
    long id;
    bool isExtended;
    bool isRtr;
    String data;
};

class CANTransceiver
{
private:
    CANPacket m_lastPacket;
    bool m_packetHasChanged;

public:
    CANTransceiver(/* args */);
    ~CANTransceiver();

    bool Begin();
    void Process();
    bool HasChanged();

    long ID();
    bool IsExtended();
    bool IsRtr();
    String Data();
};
