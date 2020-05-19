#pragma once
#include <string>
#include <cstdint>
#include "qpcpp.hpp"
#include "database/database_manager.hpp"

#define MQTT_MESSAGE_SIZE 500
#define MQTT_MESSAGE_DATA_SIZE 255

enum eSignals{
    MQTT_MESSAGE_RECIVED_SIG = QP::Q_USER_SIG,
    HW_UPDATE_SENSOR_SIG,
    HW_ADD_SENSOR_SIG,
    HW_REMOVE_SENSOR_SIG,
    APP_SET_TRESHOLD_SIG,
    APP_GET_SENSORS_SIG,
    APP_SENSOR_SETTINGS_SIG,
    RESET_DB_SIG,
    TIMEOUT_SIG,
    MQTT_START_LOOP_SIG,
    SYSTEM_ALLARM_SIG,
    DB_CALLBACK_RECIVED_SIG,
    MAX_PUB_SIG,
    DB_GET_SENSORS_SIG,
    DB_ADD_SENSOR_SIG,
    DB_ERR_SIG,
    DB_UPDATE_SENSOR_SIG,
    HANDLING_FINISHED_SIG
};

enum {ALLARM_TICKS = 100};

class MQTTMessageRecived : public QP::QEvt {
public:
    std::string m_topic;
    std::string m_message;
    MQTTMessageRecived(QP::QSignal sig);
};

class HWAddSensor : public QP::QEvt {
public:
    sensor_t m_type;
    std::string m_title;
public:
    HWAddSensor(QP::QSignal sig);
};

class HWSensorValueUpdate : public QP::QEvt {
public:
    int32_t m_id;
    int32_t m_value;

public:
    HWSensorValueUpdate(QP::QSignal sig);
};
class HWRemoveSensor : public QP::QEvt {
public:
    uint8_t m_id;

public:
    HWRemoveSensor(QP::QSignal sig);
};

class CDBCallbackEvt : public QP::QEvt {
public:
    std::string m_data;
    CDBCallbackEvt(QP::QSignal sig);
};