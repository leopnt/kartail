#pragma once

#include "logger.h"

class GlobalLogger : public Logger
{
private:
    /* data */
public:
    GlobalLogger(/* args */);
    ~GlobalLogger();
    void LogWithTag(String message, String tag);
    void Info(String message);
    void Error(String message);
};

extern GlobalLogger Log;
