#pragma once

#include <Arduino.h>
#include <SD.h>
#include "gps.h"
#include "clock.h"

#define BUFFER_LINE_LENGTH 10

class Logger
{
protected:
    File m_logFile;
    String m_logBuffer;
    u_int32_t m_logCount;
    String m_path;

    Clock *m_clock;

public:
    Logger(/* args */);
    ~Logger();
    void Init(Clock *clock, String path, String ext);

    void Push(String message);
    void PushLine(String message);
};
