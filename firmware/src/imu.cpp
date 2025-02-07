#include "imu.h"
#include "global_logger.h"
#include "exception.h"

IMU::IMU()
{
    m_accel = Adafruit_ADXL345_Unified(111);
    m_ax = 0;
    m_ay = 0;
    m_az = 0;

    m_magnet = Adafruit_HMC5883_Unified(222);
    m_mx = 0;
    m_my = 0;
    m_mz = 0;

    m_hasChanged = false;
}

IMU::~IMU()
{
}

bool IMU::Begin()
{
    if (!m_accel.begin())
    {
        Log.Error("Failed to initalize ADLX345");
        return false;
    }

    if (!m_magnet.begin())
    {
        Log.Error("Failed to initalize HMC5883");
        return false;
    }

    m_accel.setRange(ADXL345_RANGE_4_G);
    m_accel.setDataRate(ADXL345_DATARATE_100_HZ);

    itg.initialize();

    return true;
}

void IMU::Process()
{
    sensors_event_t event;
    m_accel.getEvent(&event);

    m_ax = event.acceleration.x;
    m_ay = event.acceleration.y;
    m_az = event.acceleration.z;

    m_magnet.getEvent(&event);

    m_mx = event.magnetic.x;
    m_my = event.magnetic.y;
    m_mz = event.magnetic.z;

    m_gx = (float)itg.getRotationX() / 14.375;
    m_gy = (float)itg.getRotationY() / 14.375;
    m_gz = (float)itg.getRotationZ() / 14.375;
    m_gtemperature = 35.0 + ((float)itg.getTemperature() + 13200.0) / 280.0;

    int samplingDeltaTime = int((1.0 / (float)IMU_SAMPLE_RATE) * 1000.0);
    if (millis() - m_lastMillis > samplingDeltaTime)
    {
        m_hasChanged = true;
        m_lastMillis = millis();
    }
}

bool IMU::HasChanged()
{
    if (m_hasChanged)
    {
        m_hasChanged = false;
        return true;
    }

    return false;
}

float IMU::AX() { return m_ax; }
float IMU::AY() { return m_ay; }
float IMU::AZ() { return m_az; }

float IMU::MX() { return m_mx; }
float IMU::MY() { return m_my; }
float IMU::MZ() { return m_mz; }

float IMU::GX() { return m_gx; }
float IMU::GY() { return m_gy; }
float IMU::GZ() { return m_gz; }
float IMU::GTemp() { return m_gtemperature; }

String IMU::info()
{
    String out = String("");
    sensor_t sensor;
    m_accel.getSensor(&sensor);

    out += String("Accelerometer(") + sensor.name;
    out += String(", v") + sensor.version;
    out += String(", id: ") + sensor.sensor_id;
    out += String(", min: ") + sensor.min_value + String("m/s^2");
    out += String(", max: ") + sensor.max_value + String("m/s^2");
    out += String(", resolution: ") + sensor.resolution + String("m/s^2");

    out += String(", rate: ");
    switch (m_accel.getDataRate())
    {
    case ADXL345_DATARATE_3200_HZ:
        out += String("3200");
        break;
    case ADXL345_DATARATE_1600_HZ:
        out += String("1600");
        break;
    case ADXL345_DATARATE_800_HZ:
        out += String("800");
        break;
    case ADXL345_DATARATE_400_HZ:
        out += String("400");
        break;
    case ADXL345_DATARATE_200_HZ:
        out += String("200");
        break;
    case ADXL345_DATARATE_100_HZ:
        out += String("100");
        break;
    case ADXL345_DATARATE_50_HZ:
        out += String("50");
        break;
    case ADXL345_DATARATE_25_HZ:
        out += String("25");
        break;
    case ADXL345_DATARATE_12_5_HZ:
        out += String("12.5");
        break;
    case ADXL345_DATARATE_6_25HZ:
        out += String("6.25");
        break;
    case ADXL345_DATARATE_3_13_HZ:
        out += String("3.13");
        break;
    case ADXL345_DATARATE_1_56_HZ:
        out += String("1.56");
        break;
    case ADXL345_DATARATE_0_78_HZ:
        out += String("0.78");
        break;
    case ADXL345_DATARATE_0_39_HZ:
        out += String("0.39");
        break;
    case ADXL345_DATARATE_0_20_HZ:
        out += String("0.20");
        break;
    case ADXL345_DATARATE_0_10_HZ:
        out += String("0.10");
        break;
    default:
        out += String("????");
        break;
    }
    out += String("Hz");

    out += String(", range: +/-");
    switch (m_accel.getRange())
    {
    case ADXL345_RANGE_16_G:
        out += String("16");
        break;
    case ADXL345_RANGE_8_G:
        out += String("8");
        break;
    case ADXL345_RANGE_4_G:
        out += String("4");
        break;
    case ADXL345_RANGE_2_G:
        out += String("2");
        break;
    default:
        out += String("??");
        break;
    }
    out += String("g");

    out += String("), ");

    m_magnet.getSensor(&sensor);

    out += String("Magnetometer(") + sensor.name;
    out += String(", v") + sensor.version;
    out += String(", id: ") + sensor.sensor_id;
    out += String(", min: ") + sensor.min_value + String("uT");
    out += String(", max: ") + sensor.max_value + String("uT");
    out += String(", resolution: ") + sensor.resolution + String("uT");
    out += String("), ");

    out += String("Gyroscope(ITG3205");
    out += String(", id: ") + itg.getDeviceID();
    out += String(")");

    return out;
}