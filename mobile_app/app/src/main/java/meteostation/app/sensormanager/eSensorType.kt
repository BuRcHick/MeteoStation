package meteostation.app.sensormanager

enum class eSensorType(type: String) {
    eTemperature("TEMPERATURE"),
    eHumidity("HUMIDITY"),
    ePressure("PRESSURE"),
    eOthers("OTHER")
}