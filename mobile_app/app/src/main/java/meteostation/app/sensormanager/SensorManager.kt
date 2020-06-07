package meteostation.app.sensormanager

import android.util.Log

class SensorManager() {
    private var sensorList =  ArrayList<Sensor>()

    fun addSensor(sensor: Sensor) {
        sensorList.add(sensor)
    }

    fun updateSensor(sensorId: Int, sensorValue: Int){
        var sensor = sensorList.find { it.m_id == sensorId }
        if( sensor != null) {
            sensor.m_value = sensorValue
        }
    }

    fun deleteSensor(id: Int) {
        sensorList.forEach {
            if(it.m_id == id) {
                sensorList.remove(it)
            }
        }
    }

    fun clear() {
        sensorList.clear()
    }

    fun getSensor(sensorId: Int): Sensor? {
        return sensorList.find { it.m_id == sensorId }
    }

    fun size():Int {return sensorList.size}
}