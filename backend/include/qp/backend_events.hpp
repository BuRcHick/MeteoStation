#pragma once
#include <string>
#include "qpcpp.hpp"

enum {
    MQTT_MESSAGE_RECIVED_SIG = QP::Q_USER_SIG,
    HW_UPDATE_SENSOR_SIG,
    HW_ADD_SENSOR_SIG,
    HW_REMOVE_SENSOR_SIG,
    APP_SET_TRESHOLD_SIG,
    APP_GET_SENSORS_SIG,
    APP_SENSOR_SETTINGS_SIG,
    DB_CREATED_SIG,
    DB_CB_GET_SENSORS_SIG,
    DB_CB_ADD_SENSOR_SIG,
    DB_ERR_SIG,
    TIMEOUT_SIG,
};

enum {ALLARM_TICKS = 100};

class MQTTMessageRecived : public QP::QEvt {
public:
    std::string m_topic;
    std::string m_message;
    MQTTMessageRecived(QP::QSignal sig);
};