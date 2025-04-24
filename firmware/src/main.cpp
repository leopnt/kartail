#include <Arduino.h>
#include "gps.h"
#include "imu.h"
#include "exception.h"
#include "logger.h"
#include "global_logger.h"
#include "pins.h"
#include "utils.h"
#include "esp_timer.h"
#include "obd_transceiver.h"
#include <esp_system.h>
#include "session.h"

GPS gps;
Logger gpsLog;
IMU imu;
Logger imuLog;
OBDTransceiver obdTRx;
Logger obdTRxLog;

Session session;

void setup()
{
    Serial.begin(115200);
    pinMode(BUILTIN_LED, OUTPUT);

    // let a few seconds for external Serial monitor to startup
    for (size_t i = 0; i < 20; i++)
    {
        digitalWrite(BUILTIN_LED, HIGH);
        delay(100);
        digitalWrite(BUILTIN_LED, LOW);
        delay(100);
    }

    session.Init("/session.bin");

    Log.Info("Initialize IMU...");
    if (!imu.Begin())
    {
        Log.Error("Failed to start IMU. Rebooting...");
        delay(3000);
        esp_restart();
    }
    Log.Info(imu.info());

    Log.Info("Initialize OBD Transceiver...");
    if (!obdTRx.Begin())
    {
        Raise("Failed to start OBD communication!");
    }

    Log.Init(session, "/log", "txt");

    gpsLog.Init(session, "/gps", "csv");
    gpsLog.PushLine("session,millis,datetime,latitude,longitude,altitude,n_satellites,hdop");

    u_int16_t saveEveryNSeconds = 5;
    imuLog.Init(session, "/imu", "csv", IMU_SAMPLE_RATE * saveEveryNSeconds);
    imuLog.PushLine("session,millis,ax,ay,az,mx,my,mz,gx,gy,gz");

    obdTRxLog.Init(session, "/obd", "csv", 100);
    obdTRxLog.PushLine("session,millis,pid,data");

    Log.Info("Setup complete. Begin loop...");
}

void loop()
{
    gps.Process();
    imu.Process();
    obdTRx.Process();

    if (gps.HasChanged())
    {
        gpsLog.Push(String(session.GetID()));
        gpsLog.Push(",");
        gpsLog.Push(String(millis()));
        gpsLog.Push(",");
        gpsLog.Push(ISODateUtc(gps.Year(), gps.Month(), gps.Day(), gps.Hour(), gps.Minute(), gps.Second()));
        gpsLog.Push(",");
        gpsLog.Push(String(gps.Lat(), 6));
        gpsLog.Push(",");
        gpsLog.Push(String(gps.Lng(), 6));
        gpsLog.Push(",");
        gpsLog.Push(String(gps.Ele(), 1));
        gpsLog.Push(",");
        gpsLog.Push(String(gps.NumSatellites()));
        gpsLog.Push(",");
        gpsLog.PushLine(String(gps.Hdop(), 1));
    }

    if (imu.HasChanged())
    {
        imuLog.Push(String(session.GetID()));
        imuLog.Push(",");
        imuLog.Push(String(millis()));
        imuLog.Push(",");
        imuLog.Push(String(imu.AX()));
        imuLog.Push(",");
        imuLog.Push(String(imu.AY()));
        imuLog.Push(",");
        imuLog.Push(String(imu.AZ()));
        imuLog.Push(",");
        imuLog.Push(String(imu.MX()));
        imuLog.Push(",");
        imuLog.Push(String(imu.MY()));
        imuLog.Push(",");
        imuLog.Push(String(imu.MZ()));
        imuLog.Push(",");
        imuLog.Push(String(imu.GX()));
        imuLog.Push(",");
        imuLog.Push(String(imu.GY()));
        imuLog.Push(",");
        imuLog.PushLine(String(imu.GZ()));
    }

    if (obdTRx.HasChanged())
    {
        obdTRxLog.Push(String(session.GetID()));
        obdTRxLog.Push(",");
        obdTRxLog.Push(String(millis()));
        obdTRxLog.Push(",");
        obdTRxLog.Push(String(obdTRx.ID()));
        obdTRxLog.Push(",");
        obdTRxLog.PushLine(String(obdTRx.Data(), 16));
    }
}
