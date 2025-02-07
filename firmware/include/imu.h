#pragma once

#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_HMC5883_U.h>
#include "ITG3200.h"

#define IMU_SAMPLE_RATE 50

class IMU
{
private:
    Adafruit_ADXL345_Unified m_accel;
    Adafruit_HMC5883_Unified m_magnet;
    ITG3200 itg;

    float m_ax;
    float m_ay;
    float m_az;

    float m_mx;
    float m_my;
    float m_mz;

    float m_gx;
    float m_gy;
    float m_gz;
    float m_gtemperature;

    bool m_hasChanged;

    uint32_t m_lastMillis;

public:
    IMU();
    ~IMU();

    bool Begin();

    String info();
    void Process();

    bool HasChanged();

    float AX();
    float AY();
    float AZ();

    float MX();
    float MY();
    float MZ();

    float GX();
    float GY();
    float GZ();
    float GTemp();
};
