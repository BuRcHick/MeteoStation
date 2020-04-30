#pragma once
#include <mosquittopp.h>
#include <string>
#include <bits/stdc++.h> 
#include <functional>
#include "common_status.hpp"

class CMQTTClient: public mosqpp::mosquittopp
{
private:
    std::map<std::string, std::function<void(const std::string&)> > m_topics;
public:
    CMQTTClient(const char *id, const char *host, int port);
    ~CMQTTClient();

    void on_connect(int rc);
    void on_message(const struct mosquitto_message *message);
    void on_subscribe(int mid, int qos_count, const int *granted_qos);
    void on_unsubscribe(int);
    void on_disconnect(int);

    common_status_t subscribeOnTopic(const std::string&, std::function<void(const std::string&)>);
    common_status_t unsubscribeFromTopic(const std::string&);

};