#include <cstring>
#include "mqtt/mqtt_client_api.hpp"
#include "logger/logger_api.hpp"

#define MQTT_CLIENT_PREF "MQTT Client:"
#define MQTT_CLIENT_LOG_ERROR(fmt, ...) CLogger::getLogger()->msgToLog(log_err, MQTT_CLIENT_PREF fmt, ##__VA_ARGS__)
#define MQTT_CLIENT_LOG_DEBUG(fmt, ...) CLogger::getLogger()->msgToLog(log_dbg, MQTT_CLIENT_PREF fmt, ##__VA_ARGS__)
#define MQTT_CLIENT_LOG_INFO(fmt, ...) CLogger::getLogger()->msgToLog(log_info, MQTT_CLIENT_PREF fmt, ##__VA_ARGS__)
#define MQTT_CLIENT_LOG_TRACE(fmt, ...) CLogger::getLogger()->msgToLog(log_trace, MQTT_CLIENT_PREF fmt, ##__VA_ARGS__)

#define PUBLISH_TOPIC "anime"
#define MAX_PAYLOAD 50
#define DEFAULT_KEEP_ALIVE 60

static bool s_subscribed = false;

CMQTTClient::CMQTTClient(const char *id, const char *host, int port)
	: mosquittopp(id) {
	connect(host, port, DEFAULT_KEEP_ALIVE);
}

CMQTTClient::~CMQTTClient() {
}

void CMQTTClient::on_connect(int rc) {
	if (!rc) {
        MQTT_CLIENT_LOG_DEBUG("Successfuly connected");
	}
}

void CMQTTClient::on_subscribe(int mid, int qos_count, const int *granted_qos) {
    if(!s_subscribed){
        MQTT_CLIENT_LOG_DEBUG("Successfuly subscribed");
        s_subscribed = true;
    }
}

void CMQTTClient::on_unsubscribe(int) {
	MQTT_CLIENT_LOG_DEBUG("Unsubscribed");
    s_subscribed = false;
}
void CMQTTClient::on_disconnect(int) {
	MQTT_CLIENT_LOG_DEBUG("Disconecteds");
}

void CMQTTClient::on_message(const struct mosquitto_message *message) {
	int payload_size = MAX_PAYLOAD + 1;
	char buf[payload_size];
	if (m_topics.find(message->topic) != m_topics.end()) {
		memcpy(buf, message->payload, MAX_PAYLOAD * sizeof(char));
		m_topics.find(message->topic)->second(buf);
	}
}

common_status_t CMQTTClient::subscribeOnTopic(const std::string& topic, std::function<void(const std::string&)> cb) {
	m_topics.insert(std::make_pair(topic, std::move(cb)));
	subscribe(NULL, topic.c_str());
	return cmn_success;
}
common_status_t CMQTTClient::unsubscribeFromTopic(const std::string& topic) {
	m_topics.erase(topic);
	return cmn_success;
}