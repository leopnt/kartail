#include "global_logger.h"
#include "utils.h"

GlobalLogger Log;

GlobalLogger::GlobalLogger() : Logger()
{
}

GlobalLogger::~GlobalLogger()
{
}

void GlobalLogger::LogWithTag(String message, String tag)
{
    String out = String();

    out += "[";
    if (m_clock != NULL)
        out += ISODateUtcMillis(*m_clock);
    out += "]";
    out += "[";
    out += String(millis());
    out += "]";
    out += String("[") + tag + String("]: ") + message;

    Serial.println(out);

    if (m_logFile) // prevent infinite loops in case PushLine() is called within PushLine()
        PushLine(out);
}

void GlobalLogger::Info(String message)
{
    LogWithTag(message, String("INFO"));
}

void GlobalLogger::Error(String message)
{
    LogWithTag(message, String("ERROR"));
}
