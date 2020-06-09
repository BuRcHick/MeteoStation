#ifndef _MQTT_BROKER_HPP_
#define _MQTT_BROKER_HPP_
#include "uMQTTBroker.h"

typedef void (*onRecivedCB)(String topic, const char *data, uint32_t length);

class CMQTTBroker : public uMQTTBroker {
    onRecivedCB mOnRecivedCB;

   public:
    explicit CMQTTBroker(onRecivedCB cb) : uMQTTBroker(), mOnRecivedCB{cb} {}

    virtual bool onConnect(IPAddress addr, uint16_t client_count) {
        Serial.println(addr.toString() + " connected");
        return true;
    }

    virtual bool onAuth(String username, String password) {
        Serial.println("Username/Password: " + username + "/" + password);
        return true;
    }

   private:
    virtual void onData(String topic, const char *data, uint32_t length) {
        mOnRecivedCB(topic, data, length);
    }
};

#endif  //_MQTT_BROKER_HPP_