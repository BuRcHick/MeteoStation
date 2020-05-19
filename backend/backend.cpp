#include <cstdint>
#include "qpcpp.hpp"
#include "database/database_manager.hpp"
#include "database/database_api.hpp"
#include "mqtt/mqtt_client_api.hpp"
#include "qp/database_sm.hpp"
#include "qp/backend_events.hpp"
#include "qp/mqtt_client_sm.hpp"
#include "qp/database_cb.hpp"
#include "logger/logger_api.hpp"

uint16_t PORT = 1883;
std::string ID = "MS_MQTT_ID";
std::string HOST = "localhost";

const char* g_topics[] = {
    "HW",
    "APP"
};

CMQTTClient g_mqttClient(ID.c_str());
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

static DATABASE_SM::CDatabaseSM l_database;
QP::QActive * const AO_Database = &l_database;

static DATABASE_SM::CDatabaseCB l_databaseCB;
QP::QActive * const DATABASE_SM::AO_DatabaseCB = &l_databaseCB;


static CMQTTLoop l_mqttLoop;
QP::QActive * const AO_MQTTLoop = &l_mqttLoop;

static CMQTTEvtHandler l_mqttEvtHandler;
QP::QActive * const AO_MQTTEvtHandler = &l_mqttEvtHandler;

int main() {
    //g_mqttClient = new CMQTTClient(ID.c_str());
    g_logger->setLogLevel(log_dbg);
    g_logger->setLogLevel(log_err);
    g_logger->setLogLevel(log_info);
    g_logger->setLogLevel(log_trace);
    static QP::QSubscrList subscrSto[255];
    static QF_MPOOL_EL(HWSensorValueUpdate) smallPoolSto[10];
    static QF_MPOOL_EL(HWAddSensor) medPoolSto[10];
    static QF_MPOOL_EL(MQTTMessageRecived) largePoolSto[10];
    static QP::QEvt const *database_queueSto[10];
    static QP::QEvt const *databaseCB_queueSto[10];
    static QP::QEvt const *mqttLoop_queueSto[2];
    static QP::QEvt const *mqttEcvHndlr_queueSto[10];
    QP::QF::init(); // initialize the framework and the underlying RT kernel
    QP::QF::psInit(subscrSto, Q_DIM(subscrSto)); // init publish-subscribe
    QP::QF::poolInit(smallPoolSto, sizeof(smallPoolSto), sizeof(smallPoolSto[0]));
    QP::QF::poolInit(medPoolSto, sizeof(medPoolSto), sizeof(medPoolSto[0]));
    QP::QF::poolInit(largePoolSto, sizeof(largePoolSto), sizeof(largePoolSto[0]));
    AO_Database->start(1U, // priority
                     database_queueSto, Q_DIM(database_queueSto),
                     nullptr, 0U); // no stack
    DATABASE_SM::AO_DatabaseCB->start(2U, // priority
                     databaseCB_queueSto, Q_DIM(databaseCB_queueSto),
                     nullptr, 0U); // no stack
    AO_MQTTLoop->start(3U, // priority
                     mqttLoop_queueSto, Q_DIM(mqttLoop_queueSto),
                     nullptr, 0U); // no stack
    AO_MQTTEvtHandler->start(4U, // priority
                     mqttEcvHndlr_queueSto, Q_DIM(mqttEcvHndlr_queueSto),
                     nullptr, 0U); // no stack
    return QP::QF::run();
}

void mqttClientInit(){
    for (size_t i = 0; i < Q_DIM(g_topics); i++){
        g_mqttClient.subscribeOnTopic(g_topics[i]);
    }
}

void mqttClietLoop(){
    g_mqttClient.connectToBroker(HOST.c_str(), PORT);
    mqttClientInit();
    g_mqttClient.loop_forever();
    g_mqttClient.loop_stop();
}