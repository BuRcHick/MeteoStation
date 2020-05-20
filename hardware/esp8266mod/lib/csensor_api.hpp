#pragma once

typedef void (*GET_DATA_CALLBACK)(int&);

enum sensorType{
    temperature,
    humidity,
    pressure,
};

enum sensorStatus{
    no_respond,
    active_state,
};

const char* sensorTypeToString(sensorType type) {
    switch (type)
    {
    case temperature:
        return "TEMPERATURE";
    case humidity:
        return "HUMIDITY";
    case pressure:
        return "PRESSURE";
    default:
        return "UNDEFINED";
    }
}

class CSensor
{
private:
    unsigned int m_id;
    int m_value;
    sensorType m_type;
    GET_DATA_CALLBACK m_cb;
    sensorStatus m_status;

public:
    CSensor();
    CSensor(sensorType, GET_DATA_CALLBACK);
    void setType(sensorType);
    sensorType getType() const {return m_type; };
    void setCallback(GET_DATA_CALLBACK);

    void setId(const unsigned int);
    const unsigned int getId() const { return m_id; };

    void setStatus(sensorStatus);
    sensorStatus getStatus() const { return m_status; };

    int getValue();
    
    ~CSensor();
};

CSensor::CSensor() 
    :m_cb(nullptr){}

CSensor::CSensor(sensorType _type, GET_DATA_CALLBACK _cb)
    :m_type(_type), m_cb(_cb){}

void CSensor::setType(sensorType _type){
    m_type = _type;
}

void CSensor::setCallback(GET_DATA_CALLBACK _cb){
    m_cb = _cb;
}

int CSensor::getValue(){
    if(m_cb) {
        m_cb(m_value);
    }
    return m_value;
}

void CSensor::setId(const unsigned int id) {
    m_id = id;
}

void CSensor::setStatus(sensorStatus status) {
    m_status = status;
}


CSensor::~CSensor(){
}