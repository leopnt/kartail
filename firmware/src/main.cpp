#include <Arduino.h>
#include "gps.h"
#include "imu.h"
#include "clock.h"
#include "exception.h"
#include "logger.h"
#include "global_logger.h"
#include "pins.h"
#include "utils.h"
#include "esp_timer.h"
#include "can_transceiver.h"
#include <esp_system.h>

Clock sClock;
GPS gps;
Logger gpsLog;
IMU imu;
Logger imuLog;
CANTransceiver canTRx;
Logger canTRxLog;

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

    Log.Info("Initialize IMU...");
    if (!imu.Begin())
    {
        Log.Error("Failed to start IMU. Rebooting...");
        delay(3000);
        esp_restart();
    }
    Log.Info(imu.info());

    Log.Info("Initialize CAN Transceiver...");
    if (!canTRx.Begin())
    {
        Raise("Failed to start CAN transceiver serial communication!");
    }

    Log.Info("Waiting for first GPS fix...");
    while (!gps.IsValid())
        gps.Process();

    Log.Info("GPS fix acquired!");
    Log.Info("Syncing time...");
    sClock.Sync(gps);

    Log.Info("Initalize logging...");
    Log.Init(&sClock, "/log", "txt");

    gpsLog.Init(&sClock, "/gps", "csv");
    gpsLog.PushLine("datetime,millis,latitude,longitude,altitude,n_satellites,hdop");

    u_int16_t saveEveryNSeconds = 5;
    imuLog.Init(&sClock, "/imu", "csv", IMU_SAMPLE_RATE * saveEveryNSeconds);
    imuLog.PushLine("datetime,millis,ax,ay,az,mx,my,mz,gx,gy,gz");

    canTRxLog.Init(&sClock, "/can", "csv", 100);
    canTRxLog.PushLine("datetime,millis,id,is_extended,is_rtr,data");

    Log.Info("Setup complete. Begin loop...");
}

void loop()
{
    gps.Process();

    if (gps.HasChanged())
    {
        sClock.Sync(gps);

        gpsLog.Push(ISODateUtcMillis(sClock));
        gpsLog.Push(",");
        gpsLog.Push(String(millis()));
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

    imu.Process();

    if (imu.HasChanged())
    {
        imuLog.Push(ISODateUtcMillis(sClock));
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

    canTRx.Process();

    if (canTRx.HasChanged())
    {
        canTRxLog.Push(ISODateUtcMillis(sClock));
        canTRxLog.Push(",");
        canTRxLog.Push(String(millis()));
        canTRxLog.Push(",");
        canTRxLog.Push(String(canTRx.ID()));
        canTRxLog.Push(",");
        canTRxLog.Push(String((int)canTRx.IsExtended()));
        canTRxLog.Push(",");
        canTRxLog.Push(String((int)canTRx.IsRtr()));
        canTRxLog.Push(",");
        canTRxLog.PushLine(canTRx.Data());
    }
}
