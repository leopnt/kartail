#include "session.h"
#include "pins.h"
#include "exception.h"
#include "global_logger.h"

Session::Session() : m_id(0)
{
}

Session::~Session()
{
}

void Session::CreateSessionFile(String filename)
{
    Log.Info(filename + " does not exists. creating...");
    File newFile = SD.open(filename, FILE_WRITE);

    if (!newFile)
        Raise("Failed to create file: " + filename);

    newFile.write((uint8_t *)&m_id, sizeof(m_id));
    newFile.flush();
    newFile.close();
    Log.Info("Wrote default session id to file: " + m_id);
}

void Session::WriteSessionIdToFile(String filename)
{
    File file = SD.open(filename, FILE_WRITE);
    if (!file)
        Raise("Failed to open " + filename);

    file.write((uint8_t *)&m_id, sizeof(m_id));
    file.flush();
    file.close();
    Log.Info("Wrote session id to file: " + String(m_id));
}

void Session::ReadSessionIdFromFile(String filename)
{
    File file = SD.open(filename, FILE_READ);
    if (!file)
        Raise("Failed to open " + filename);

    file.read((uint8_t *)&m_id, sizeof(m_id));

    Log.Info("Parsed last session id from file: " + String(m_id));

    m_file.close();
}

void Session::Init(String filename)
{
    if (!SD.begin(SD_SC))
        Raise("Failed to initialize SD Card!");

    if (!SD.exists(filename))
        CreateSessionFile(filename);

    ReadSessionIdFromFile(filename);

    m_id += 1;
    Log.Info("Set current session id to: " + String(m_id));

    WriteSessionIdToFile(filename);
}

uint32_t Session::GetID() const
{
    return m_id;
}
