#pragma once
#include <mosquittopp.h>
#include <string>
#include <bits/stdc++.h> 
#include <functional>
#include "common_status.hpp"

class CMQTTClient: public mosqpp::mosquittopp
{
private:
    std::set<std::string> m_topics;
public:
    CMQTTClient(const char *id);
    ~CMQTTClient();

    void on_connect(int rc);
    void on_message(const struct mosquitto_message *message);
    void on_subscribe(int mid, int qos_count, const int *granted_qos);
    void on_unsubscribe(int);
    void on_disconnect(int);
    void on_publish(int);

    common_status_t subscribeOnTopic(const std::string&);
    common_status_t unsubscribeFromTopic(const std::string&);

    common_status_t connectToBroker(const char *host, int port);

    common_status_t publishMessage(const char *topic, const char* message);

};