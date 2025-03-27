#pragma once

#include <Arduino.h>
#include <SD.h>
#include "gps.h"
#include "session.h"

#define DEFAULT_BUFFER_LINE_LENGTH 10

class Logger
{
protected:
    File m_logFile;
    String m_logBuffer;
    u_int32_t m_logCount;
    String m_path;
    uint16_t m_bufferLineLength;

public:
    Logger(/* args */);
    ~Logger();
    void Init(Session const &session, String path, String ext, uint16_t bufferLineLength = DEFAULT_BUFFER_LINE_LENGTH);

    void Push(String message);
    void PushLine(String message);
};
