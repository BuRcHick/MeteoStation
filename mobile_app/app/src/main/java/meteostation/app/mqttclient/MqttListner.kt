package meteostation.app.mqttclient

interface MqttListner {
    fun onMessageRecived(topic: String, message: String)
}