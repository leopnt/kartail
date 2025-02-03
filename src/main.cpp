#include <Arduino.h>
#include "gps.h"
#include "clock.h"
#include "exception.h"
#include "logger.h"
#include "global_logger.h"
#include "pins.h"
#include "utils.h"

#include "git_version.h" // generated via pre platformio script

Clock sClock;
GPS gps;
Logger gpsLog;

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

    Log.Info(String("Starting kartail... Commit Version: ") + String(GIT_VERSION));
    Log.Info("Waiting for first GPS fix...");
    while (!gps.IsValid())
        gps.Process();

    Log.Info("GPS fix acquired!");
    Log.Info("Syncing time...");
    sClock.Sync(gps);

    Log.Info("Initalize logging...");
    Log.Init(&sClock, "/" + String(GIT_VERSION) + "/log", "txt");

    gpsLog.Init(&sClock, "/" + String(GIT_VERSION) + "/gps", "csv");
    gpsLog.Push("datetime");
    gpsLog.Push(",");
    gpsLog.Push("millis");
    gpsLog.Push(",");
    gpsLog.Push("latitude");
    gpsLog.Push(",");
    gpsLog.Push("longitude");
    gpsLog.Push(",");
    gpsLog.PushLine("elevation");

    Log.Info("Setup complete. Begin loop...");
}

void loop()
{
    gps.Process();

    if (gps.HasChanged())
    {
        sClock.Sync(gps);
        String gpsData = gps.ToString();

        gpsLog.Push(ISODateUtcMillis(sClock));
        gpsLog.Push(",");
        gpsLog.Push(String(millis()));
        gpsLog.Push(",");
        gpsLog.Push(String(gps.Lat()));
        gpsLog.Push(",");
        gpsLog.Push(String(gps.Lng()));
        gpsLog.Push(",");
        gpsLog.PushLine(String(gps.Ele()));
    }
}
