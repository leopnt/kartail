#pragma once

#include <Arduino.h>
#include <SD.h>

class Session
{
private:
    uint32_t m_id;
    File m_file;

    void CreateSessionFile(String filename);
    void WriteSessionIdToFile(String filename);
    void ReadSessionIdFromFile(String filename);

public:
    Session();
    ~Session();

    void Init(String filename);
    uint32_t GetID() const;
};
