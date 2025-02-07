#include "logger.h"
#include "exception.h"
#include "pins.h"
#include "utils.h"
#include "global_logger.h"

Logger::Logger() : m_bufferLineLength(DEFAULT_BUFFER_LINE_LENGTH)
{
}

Logger::~Logger()
{
    m_clock = NULL;
    SD.end();
}

void mkdirp(String dir)
{
    if (dir.length() == 0 || dir == "/")
        return;

    if (dir.endsWith("/"))
        dir.remove(dir.length() - 1);

    int lastSlash = dir.lastIndexOf('/');
    if (lastSlash > 0)
    {
        String parentDir = dir.substring(0, lastSlash);

        if (!SD.exists(parentDir))
        {
            mkdirp(parentDir);
        }
    }

    if (!SD.exists(dir))
    {
        Log.Info(dir + " does not exist. Creating...");
        if (!SD.mkdir(dir))
        {
            Raise("Failed to create " + dir);
            return;
        }
    }
}

void Logger::Init(Clock *clock, String path, String ext, uint16_t bufferLineLength)
{
    m_bufferLineLength = bufferLineLength;
    m_clock = clock;
    m_path = path;

    if (!SD.begin(SD_SC))
        Raise("Failed to initialize SD Card!");

    mkdirp(path);

    String now = ISODateUtc(*m_clock);
    now.replace(String(':'), "");

    String filename = m_path + "/" + now + "." + ext;

    Log.Info("Opening file: " + filename);

    m_logFile = SD.open(filename, FILE_WRITE);
    if (!m_logFile)
        Raise("Failed to open log file!");
}

void Logger::Push(String message)
{
    m_logBuffer += message;
}

void Logger::PushLine(String message)
{
    m_logBuffer += message;
    m_logBuffer += "\n";

    if (m_logCount >= m_bufferLineLength)
    {
        if (m_logFile)
        {
            m_logFile.print(m_logBuffer);
            m_logFile.flush();

            m_logBuffer.clear();
            m_logCount = 0;

            Log.Info(String("Wrote ") + m_bufferLineLength + String(" lines to file: ") + m_path + +"/" + m_logFile.name());
        }
        else
        {
            Raise("Error writing to SD card!");
        }
    }

    m_logCount++;
}
