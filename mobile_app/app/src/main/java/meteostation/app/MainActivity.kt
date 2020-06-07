package meteostation.app

import android.content.Context
import android.net.wifi.WifiManager
import android.os.Bundle
import android.text.format.Formatter
import android.util.JsonReader
import android.util.Log
import android.view.Menu
import android.widget.TextView
import com.google.android.material.floatingactionbutton.FloatingActionButton
import com.google.android.material.snackbar.Snackbar
import com.google.android.material.navigation.NavigationView
import androidx.navigation.findNavController
import androidx.navigation.ui.AppBarConfiguration
import androidx.navigation.ui.navigateUp
import androidx.navigation.ui.setupActionBarWithNavController
import androidx.navigation.ui.setupWithNavController
import androidx.drawerlayout.widget.DrawerLayout
import androidx.appcompat.app.AppCompatActivity

import androidx.appcompat.widget.Toolbar
import androidx.core.view.marginTop
import kotlinx.android.synthetic.main.fragment_home.*
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.async
import kotlinx.coroutines.channels.Channel
import kotlinx.coroutines.launch
import meteostation.app.mqttclient.MQTTClient
import meteostation.app.mqttclient.MqttListner
import meteostation.app.sensormanager.Sensor
import meteostation.app.sensormanager.SensorManager
import meteostation.app.sensormanager.eSensorType
import org.json.JSONArray
import org.json.JSONObject
import java.net.InetAddress

class MainActivity : AppCompatActivity(), MqttListner {

    private val mqtt_subscribed_topics = arrayOf("SESNORS_LIST", "SENSOR_UPDATE", "LAST_SENSOR")
    private var sensorManager = SensorManager()
    private var mqttClient = MQTTClient(this)
    private var isInit = false
    private var isConnect = false
    private var uiSensors = ArrayList<String>()


    private lateinit var appBarConfiguration: AppBarConfiguration

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        val toolbar: Toolbar = findViewById(R.id.toolbar)
        setSupportActionBar(toolbar)

        val drawerLayout: DrawerLayout = findViewById(R.id.drawer_layout)
        val navView: NavigationView = findViewById(R.id.nav_view)
        val navController = findNavController(R.id.nav_host_fragment)
        // Passing each menu ID as a set of Ids because each
        // menu should be considered as top level destinations.
        appBarConfiguration = AppBarConfiguration(setOf(
                R.id.nav_home, R.id.nav_gallery, R.id.nav_slideshow), drawerLayout)
        setupActionBarWithNavController(navController, appBarConfiguration)
        navView.setupWithNavController(navController)
        connectToMqttBroker(applicationContext)
    }

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        // Inflate the menu; this adds items to the action bar if it is present.
        menuInflater.inflate(R.menu.main, menu)
        return true
    }

    override fun onSupportNavigateUp(): Boolean {
        val navController = findNavController(R.id.nav_host_fragment)
        return navController.navigateUp(appBarConfiguration) || super.onSupportNavigateUp()
    }

    private fun connectToMqttBroker(context: Context){
        CoroutineScope(Dispatchers.IO).launch {
            var list = ArrayList<String>()
            val wm: WifiManager? = context.getSystemService(Context.WIFI_SERVICE) as WifiManager?
            if(wm != null){
                var ip: String? = Formatter.formatIpAddress(wm!!.connectionInfo.ipAddress)
                if(ip != null) {
                    val pref = ip.substring(0, ip.lastIndexOf('.') + 1)
                    async {
                        for (i in 0..255) {
                            async {
                                val newIp = pref + i.toString()
                                if(newIp != ip) {
                                    val address = InetAddress.getByName(newIp)
                                    try {
                                        if(address.isReachable(1000)) {
                                            Log.d("SUCCES", newIp)
                                            list.add(address.hostAddress)
                                        } else {
                                        }
                                    } catch (e: Exception){
                                    }
                                }
                            }
                        }
                    }.await()
                    val channel = Channel<Boolean>()
                    list.forEach{
                        Log.d("METEOSTATION", "Truing to connect to: $it")
                        mqttClient.createClient("tcp://" + it + ":1883", "Android", context)
                        mqttClient.connect(it, channel)
                        if(channel.receive() == true) {
                            for (topic in mqtt_subscribed_topics){
                                mqttClient.subscribe(topic, channel)
                                if(channel.receive()){

                                }
                            }
                            channel.close()
                            getSensors()
                            return@launch
                        }
                    }
                    channel.close()
                }
            }
        }

    }

    override fun onMessageRecived(topic: String, message: String) {
        if(isConnect) {
            when (topic) {
                "SESNORS_LIST" -> {
                    if(isInit) {
                        isInit = false
                        sensorManager.clear()
                    }
                    var array = JSONArray(message)
                    var jsonSensor = JSONObject(array.get(0).toString())
                    var sensor = Sensor(jsonSensor.getInt("m_id"), jsonSensor.getString("m_type"), jsonSensor.getInt("m_status"), jsonSensor.getInt("m_value"))
                    sensorManager.addSensor(sensor)
                    if(uiSensors.find { it == sensor.m_type } == null) {
                        uiSensors.add(sensor.m_type)
                    }

                }
                "LAST_SENSOR" -> {
                    isInit = true

                }
                "SENSOR_UPDATE" -> {
                    if(isInit) {
                        var array = JSONArray(message)
                        var jsonSensor = JSONObject(array.get(0).toString())
                        sensorManager.updateSensor(jsonSensor.getInt("m_id"), jsonSensor.getInt("m_value"))
                        var tempView = findViewById<TextView>(R.id.temperature)
                        var humView = findViewById<TextView>(R.id.humidity)
                        var prView = findViewById<TextView>(R.id.pressure)
                        var summ = 0
                        var count = 0
                        for (index in 0..sensorManager.size()) {
                            val sensor = sensorManager.getSensor(index)
                            if(sensor != null && sensor.m_type == "TEMPERATURE") {
                                summ += sensor.m_value
                                count++
                            }
                        }
                        if(count > 0) {
                            tempView.text = ((summ/count).toDouble()/100).toString()
                        }
                        count = 0
                        summ = 0
                        for (index in 0..sensorManager.size()) {
                            val sensor = sensorManager.getSensor(index)
                            if(sensor != null && sensor.m_type == "HUMIDITY") {
                                summ += sensor.m_value
                                count++
                            }
                        }
                        if(count > 0) {
                            humView.text = ((summ/count).toDouble()/100).toString()
                        }

                        count = 0
                        summ = 0
                        for (index in 0..sensorManager.size()) {
                            val sensor = sensorManager.getSensor(index)
                            if(sensor != null && sensor.m_type == "PRESSURE") {
                                summ += sensor.m_value
                                count++
                            }
                        }
                        if(count > 0) {
                            prView.text = ((summ/count).toDouble()/100).toString()
                        }
                    }
                }
            }
        }

    }

    fun getSensors() {
        isConnect = true
        mqttClient.publish("GET_SENSORS", "")
    }
}
/*
    <TextView
        android:id="@+id/text_home"
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:layout_marginStart="8dp"
        android:layout_marginTop="8dp"
        android:layout_marginEnd="8dp"
        android:textAlignment="center"
        android:textSize="20sp"
        app:layout_constraintBottom_toBottomOf="parent"
        app:layout_constraintEnd_toEndOf="parent"
        app:layout_constraintHorizontal_bias="1.0"
        app:layout_constraintStart_toStartOf="parent"
        app:layout_constraintTop_toTopOf="parent"
        app:layout_constraintVertical_bias="0.494" />
* */
