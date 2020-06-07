package meteostation.app.sensormanager

data class Sensor(val m_id: Int, val m_type: String, val m_status : Int, var m_value: Int)

data class UpdateSensor(val m_id: Int, val m_value: Int)