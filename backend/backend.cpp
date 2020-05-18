#include <cstdint>
#include "qpcpp.hpp"
#include "database/database_manager.hpp"
#include "database/database_api.hpp"
#include "mqtt/mqtt_client_api.hpp"
#include "qp/database_sm.hpp"
#include "qp/backend_events.hpp"
#include "logger/logger_api.hpp"

uint16_t PORT = 1883;
std::string ID = "MS_MQTT_ID";
std::string HOST = "localhost";

const char* g_topics[] = {
    "HW",
    "APP"
};

CMQTTClient* g_mqttClient = nullptr;
CLogger* g_logger = CLogger::getLogger();


void mqttClientInit();
void mqttClietLoop();

extern "C" Q_NORETURN Q_onAssert(char const * const module, int_t const loc) {
    g_logger->msgToLog(log_err, "Assertion failed in %s : %d", module, loc);
    exit(-1);
}
void QP::QF::onStartup(void) {}
void QP::QF::onCleanup(void) {}
void QP::QF_onClockTick(void) {
    QF::TICK_X(0U, nullptr);  // perform the QF clock tick processing
}

int main() {
    g_mqttClient = new CMQTTClient(ID.c_str());
    static QP::QSubscrList subscrSto[255];
    static QF_MPOOL_EL(MQTTMessageRecived) medPoolSto[10];
    QP::QF::init(); // initialize the framework and the underlying RT kernel
    QP::QF::psInit(subscrSto, Q_DIM(subscrSto)); // init publish-subscribe
    QP::QF::poolInit(medPoolSto, sizeof(medPoolSto), sizeof(medPoolSto[0]));
    g_logger->setLogLevel(log_dbg);
    g_logger->setLogLevel(log_err);
    g_logger->setLogLevel(log_info);
    g_logger->setLogLevel(log_trace);
    
    mqttClietLoop();
    delete g_mqttClient;
    return 0;
}

void mqttClientInit(){
    for (size_t i = 0; i < Q_DIM(g_topics); i++){
        g_mqttClient->subscribeOnTopic(g_topics[i]);
    }
}

void mqttClietLoop(){
    g_mqttClient->connectToBroker(HOST.c_str(), PORT);
    mqttClientInit();
    while(1)
    {
        int rc = g_mqttClient->loop();
        if (rc)
        {
            g_mqttClient->reconnect();
        }
        else
            mqttClientInit();
    }
    g_mqttClient->loop_forever();
    //while(true) {
    //    g_mqttClient.loop();
    //}
}

// /*

// ifstream config("backend_mqtt.cfg");
// std::string line = "";
// std::string host = "";
// std::string id = "";
// int port = 0;

// while(getline(config, line)) {
//     if(line.find("host=")){
//         host = line.substr(strlen("host=")));
//     }
//     if(line.find("port=")){
//         port = line.substr(strlen("port=")));
//     }
//     if(line.find("id=")){
//         id = std::stoi(line.substr(strlen("id="))));
//     }
// }
// g_mqttClient.connectToBroker(host.c_str(), port);
// AO_MQTTManager->POST(Q_NEW(QEvt, CONFIG_FINISHED), this);


// #include "backend.hpp"
// #include "database/database_manager.hpp"
// #include "database/database_api.hpp"
// #include "logger/logger_api.hpp"

// int main() {
//     static QP::QSubscrList subscrSto[255];

//     QP::QF::init(); // initialize the framework and the underlying RT kernel


//     QP::QF::psInit(subscrSto, Q_DIM(subscrSto)); // init publish-subscribe

//     // start the active objects...
//     // NOTE: AO_Tunnel is the GUI Active Object
//     BACKEND::AO_Tunnel ->start(1U, // priority
//                       (QP::QEvt const **)0, 0U,    // no evt queue
//                       nullptr, 0U); // no stack for GUI thread
// }
// */