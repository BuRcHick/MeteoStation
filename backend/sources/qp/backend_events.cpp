#include "qp/backend_events.hpp"

Q_DEFINE_THIS_FILE

MQTTMessageRecived::MQTTMessageRecived(QP::QSignal _sig) 
: QEvt() {
    this->sig = _sig;
}
HWAddSensor::HWAddSensor(QP::QSignal _sig) 
: QEvt() { 
    this->sig = _sig; 
}
HWSensorValueUpdate::HWSensorValueUpdate(QP::QSignal _sig) 
: QEvt() { 
    this->sig = _sig; 
}
HWRemoveSensor::HWRemoveSensor(QP::QSignal _sig)
: QEvt() { 
    this->sig = _sig; 
}