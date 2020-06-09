#ifndef _ISENSOR_HPP_
#define _ISENSOR_HPP_

#include <string>

#define VALUE_PRESCALLER 100

enum eSensorType{
    eTemperature = 0,
    eHumidity,
    ePressure,
    eOther,
};

enum eSensorStatus {
    eNotRepond = 0,
    eActive,
};

const char* sensorTypeToString(eSensorType type) {
    switch (type)
    {
    case eTemperature:
        return "TEMPERATURE";
    case eHumidity:
        return "HUMIDITY";
    case ePressure:
        return "PRESSURE";
    default:
        return "UNDEFINED";
    }
}

class ISensor {
    eSensorType mType;
    eSensorStatus mStatus;

   public:
    const eSensorType getType() const { return mType; };

    void updateStatus(const eSensorStatus newStatus) { mStatus = newStatus; };
    const eSensorStatus getStatus() const { return mStatus; };

    virtual int getValue() = 0;
    virtual ~ISensor(){};

   protected:
    ISensor(const eSensorType _type): mType{_type}, mStatus{eNotRepond} {}
};

#endif  //_ISENSOR_HPP_