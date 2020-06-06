package meteostation.app

import android.content.Context
import android.net.wifi.WifiManager
import android.os.Bundle
import android.text.format.Formatter
import android.util.Log
import android.view.Menu
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
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers.IO
import kotlinx.coroutines.async
import kotlinx.coroutines.channels.Channel
import kotlinx.coroutines.launch
import java.net.InetAddress

class MainActivity : AppCompatActivity() {

    private lateinit var appBarConfiguration: AppBarConfiguration
    private var mqttClient = MQTTClient()

    fun connectToMqttBroker(){
        CoroutineScope(IO).launch {
            var list = ArrayList<String>()
            val wm: WifiManager? = applicationContext.getSystemService(Context.WIFI_SERVICE) as WifiManager?
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
                                            list.add(address.hostAddress)
                                        } else {
                                        }
                                    } catch (e: Exception){
                                    }
                                }
                            }
                        }
                    }.await()
                }
            }
            val channel = Channel<Boolean>()
            list.forEach{
                mqttClient.createMqttClient(applicationContext, it, "MeteostationAndroid")
                Log.d("METEOSTATION", "Truing to connect to: $it")
                mqttClient.connect("tcp://" + it + ":1883", "AndroidID", applicationContext, channel)
                if(channel.receive()) {
                    return@launch
                }
            }
        }

    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        val toolbar: Toolbar = findViewById(R.id.toolbar)
        setSupportActionBar(toolbar)

        val fab: FloatingActionButton = findViewById(R.id.fab)
        fab.setOnClickListener { view ->
            Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                    .setAction("Action", null).show()
        }
        val drawerLayout: DrawerLayout = findViewById(R.id.drawer_layout)
        val navView: NavigationView = findViewById(R.id.nav_view)
        val navController = findNavController(R.id.nav_host_fragment)
        // Passing each menu ID as a set of Ids because each
        // menu should be considered as top level destinations.
        appBarConfiguration = AppBarConfiguration(setOf(
                R.id.nav_home, R.id.nav_gallery, R.id.nav_slideshow), drawerLayout)
        setupActionBarWithNavController(navController, appBarConfiguration)
        navView.setupWithNavController(navController)
        connectToMqttBroker()
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
}
