#include <cstring>
#include "mqtt/mqtt_client_api.hpp"
#include "logger/logger_api.hpp"
#include "qp/backend_events.hpp"

#define MQTT_CLIENT_PREF "MQTT Client:"
#define MQTT_CLIENT_LOG_ERROR(fmt, ...) CLogger::getLogger()->msgToLog(log_err, MQTT_CLIENT_PREF fmt, ##__VA_ARGS__)
#define MQTT_CLIENT_LOG_DEBUG(fmt, ...) CLogger::getLogger()->msgToLog(log_dbg, MQTT_CLIENT_PREF fmt, ##__VA_ARGS__)
#define MQTT_CLIENT_LOG_INFO(fmt, ...) CLogger::getLogger()->msgToLog(log_info, MQTT_CLIENT_PREF fmt, ##__VA_ARGS__)
#define MQTT_CLIENT_LOG_TRACE(fmt, ...) CLogger::getLogger()->msgToLog(log_trace, MQTT_CLIENT_PREF fmt, ##__VA_ARGS__)

#define MQTT_SUCCESS_CHECK(c, r, fmt, ...) {			\
	if(!(c)){										\
		MQTT_CLIENT_LOG_ERROR(fmt, ##__VA_ARGS__);	\
		return (r);									\
	}}

#define PUBLISH_TOPIC "anime"
#define MAX_PAYLOAD 50
#define DEFAULT_KEEP_ALIVE 60

static bool s_subscribed = false;

CMQTTClient::CMQTTClient(const char *id)
	: mosquittopp(id) {
	mosqpp::lib_init();
}

CMQTTClient::~CMQTTClient() {
	//mosquitto_loop_stop();
	mosqpp::lib_cleanup();
	MQTT_CLIENT_LOG_INFO("Client deleted");
}

void CMQTTClient::on_connect(int rc) {
	if (!rc) {
        MQTT_CLIENT_LOG_DEBUG("Successfuly connected");
	}
}

void CMQTTClient::on_subscribe(int mid, int qos_count, const int *granted_qos) {
    //if(!s_subscribed){
        MQTT_CLIENT_LOG_DEBUG("Successfuly subscribed");
        s_subscribed = true;
    //}
}

void CMQTTClient::on_unsubscribe(int) {
	MQTT_CLIENT_LOG_INFO("Unsubscribed");
    s_subscribed = false;
}

void CMQTTClient::on_disconnect(int) {
	MQTT_CLIENT_LOG_INFO("Disconecteds");
}

void CMQTTClient::on_publish(int ID) {
	MQTT_CLIENT_LOG_INFO("%d successfuly published");
}

void CMQTTClient::on_message(const struct mosquitto_message *message) {
	if(m_topics.find(message->topic) != m_topics.end()){
		MQTTMessageRecived* msg = Q_NEW(MQTTMessageRecived, MQTT_MESSAGE_RECIVED_SIG);//QP::Q_NEW(MQTTMessageRecived, MQTT_MESSAGE_RECIVED_SIG);
		msg->m_topic = (char*)message->topic;
		msg->m_message = (char*)message->payload;
		MQTT_CLIENT_LOG_DEBUG("MQTT Message recived: topic = %s data = %s", msg->m_topic.c_str(), msg->m_message.c_str());
    	QP::QF::PUBLISH(msg, nullptr);
	}
}

common_status_t CMQTTClient::subscribeOnTopic(const std::string& topic) {
	if(m_topics.find(topic) == m_topics.end()){
		MQTT_CLIENT_LOG_DEBUG("Adding topic:%s", topic.c_str());
		m_topics.insert(topic);
		subscribe(NULL, topic.c_str());
	}
	return cmn_success;
}

common_status_t CMQTTClient::unsubscribeFromTopic(const std::string& topic) {
	m_topics.erase(topic);
	return cmn_success;
}

common_status_t CMQTTClient::connectToBroker(const char *host, int port){
	connect_async(host, port, DEFAULT_KEEP_ALIVE);
	return cmn_success;
}

common_status_t CMQTTClient::publishMessage(const char *topic, const uint8_t *message, const uint16_t size){
	MQTT_SUCCESS_CHECK(publish(NULL, topic, size, message, 1, false) == MOSQ_ERR_SUCCESS, error_unknown, "MQTT Publsih error");
	return cmn_success;
}