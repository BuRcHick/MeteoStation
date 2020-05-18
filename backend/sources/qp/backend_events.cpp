#include "qp/backend_events.hpp"
Q_DEFINE_THIS_FILE

MQTTMessageRecived::MQTTMessageRecived(QP::QSignal _sig)
        :QEvt(){
                this->sig = _sig;
        }