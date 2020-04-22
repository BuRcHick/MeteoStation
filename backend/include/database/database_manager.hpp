#pragma once
#include <string>
#include <stdint.h>
#include <memory>
#include "common_status.hpp"

typedef enum {
    temperature_sensor = 1,
    humidity_sensor,
    preasure_sensor,
    other
}sensor_t;

typedef struct {
    uint8_t id;
    std::string title;
    sensor_t type;
    int value;
} senosrInfo_t;

char* sensorTypeToString(const sensor_t);

namespace DatabaseManagerN {
    common_status_t createrDB(const std::string&);
    common_status_t addNewSensor(const std::string&, const sensor_t);
    common_status_t removeSensor(const uint8_t);
    common_status_t addNewType(const std::string&);
    common_status_t removeType(const uint8_t);
    common_status_t changeSensorSettings(const uint8_t, const std::string&, const sensor_t);
    common_status_t updateSensorValue(const uint8_t, const int);

    common_status_t getSensorInfo(const uint8_t);
}