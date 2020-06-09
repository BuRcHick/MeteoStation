#ifndef _CBME200_SENSOR_API_HPP
#define _CBME200_SENSOR_API_HPP
#include "Adafruit_BME280.h"
#include "Adafruit_Sensor.h"
#include "isensor.hpp"

class CBME200TempSensor : public ISensor {
   private:
    Adafruit_BME280& mBme200;

   public:
    virtual int getValue() {
        return (int)(mBme200.readTemperature() * (VALUE_PRESCALLER));
    };

    explicit CBME200TempSensor(Adafruit_BME280& _bme200)
        : ISensor(eTemperature), mBme200(_bme200) {
        updateStatus((eSensorStatus)mBme200.begin());
    };

    ~CBME200TempSensor(){};
};

class CBME200HumSensor : public ISensor {
   private:
    Adafruit_BME280& mBme200;

   public:
    virtual int getValue() {
        return (int)(mBme200.readHumidity() * (VALUE_PRESCALLER));
    };
    explicit CBME200HumSensor(Adafruit_BME280& _bme200)
        : ISensor(eHumidity), mBme200(_bme200) {
        updateStatus((eSensorStatus)mBme200.begin());
    };

    ~CBME200HumSensor(){};
};

class CBME200PresSensor : public ISensor {
   private:
    Adafruit_BME280& mBme200;

   public:
    virtual int getValue() {
        return (int)(mBme200.readPressure() * (VALUE_PRESCALLER));
    };
    explicit CBME200PresSensor(Adafruit_BME280& _bme200)
        : ISensor(ePressure), mBme200(_bme200) {
        updateStatus((eSensorStatus)mBme200.begin());
    };

    ~CBME200PresSensor(){};
};

#endif  //_CBME200_HUM_SENSOR_API_HPP