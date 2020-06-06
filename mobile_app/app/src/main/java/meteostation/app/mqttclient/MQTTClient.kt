package meteostation.app.mqttclient

import android.content.Context
import android.util.Log
import com.beust.klaxon.JsonReader
import com.beust.klaxon.Klaxon
import kotlinx.coroutines.async
import org.eclipse.paho.android.service.MqttAndroidClient
import org.eclipse.paho.client.mqttv3.*
import java.util.*
import kotlinx.coroutines.channels.Channel
import kotlinx.coroutines.*
import kotlinx.coroutines.Dispatchers.IO
import meteostation.app.sensormanager.Sensor
import meteostation.app.sensormanager.UpdateSensor
import meteostation.app.sensormanager.eSensorType
import java.io.StringReader
import kotlin.collections.ArrayList


class MQTTClient(mqttListner: MqttListner) {
    private lateinit var client: MqttAndroidClient
    private var uniqueID:String? = null
    private val PREF_UNIQUE_ID = "PREF_UNIQUE_ID"
    private val mMqttListner: MqttListner = mqttListner
    init {

    }

    fun createClient(host: String, id: String, context: Context){
        client = MqttAndroidClient(context, host, id(context) + id)
        client.setCallback(object: MqttCallbackExtended {
            override fun connectComplete(b:Boolean, s:String) {
                Log.w("mqtt", s)
            }
            override fun connectionLost(throwable:Throwable) {
            }
            override fun messageArrived(topic:String, mqttMessage: MqttMessage) {
                mMqttListner.onMessageRecived(topic, mqttMessage.toString())
            }
            override fun deliveryComplete(iMqttDeliveryToken: IMqttDeliveryToken) {
            }
        })

    }

    fun connect(host: String, channel: Channel<Boolean>) {
        val mqttConnectOptions = MqttConnectOptions()
        mqttConnectOptions.setAutomaticReconnect(true)
        mqttConnectOptions.setCleanSession(false)
        try
        {
            client.connect(mqttConnectOptions, null, object: IMqttActionListener {
                override fun onSuccess(asyncActionToken:IMqttToken) {
                    val disconnectedBufferOptions = DisconnectedBufferOptions()
                    disconnectedBufferOptions.setBufferEnabled(true)
                    disconnectedBufferOptions.setBufferSize(100)
                    disconnectedBufferOptions.setPersistBuffer(false)
                    disconnectedBufferOptions.setDeleteOldestMessages(false)
                    client.setBufferOpts(disconnectedBufferOptions)
                    Log.w("Mqtt", "Connectted to: " + host)
                    CoroutineScope(IO).launch { async {channel.send(true)} }
                }
                override fun onFailure(asyncActionToken:IMqttToken, exception:Throwable) {
                    Log.w("Mqtt", "Failed to connect to: " + host + exception.toString())
                    CoroutineScope(IO).launch { async {channel.send(false)} }
                }
            })
        }
        catch (ex:MqttException) {
            ex.printStackTrace()
        }
    }

    fun disconnect(){
        try {
            client.disconnect(null,object :IMqttActionListener{
                /**
                 * This method is invoked when an action has completed successfully.
                 * @param asyncActionToken associated with the action that has completed
                 */
                override fun onSuccess(asyncActionToken: IMqttToken?) {
                }

                /**
                 * This method is invoked when an action fails.
                 * If a client is disconnected while an action is in progress
                 * onFailure will be called. For connections
                 * that use cleanSession set to false, any QoS 1 and 2 messages that
                 * are in the process of being delivered will be delivered to the requested
                 * quality of service next time the client connects.
                 * @param asyncActionToken associated with the action that has failed
                 */
                override fun onFailure(asyncActionToken: IMqttToken?, exception: Throwable?) {
                }

            })
        }
        catch (ex:MqttException) {
            System.err.println("Exception disconnect")
            ex.printStackTrace()
        }
    }

    // Subscribe to topic
    fun subscribe(topic: String, channel: Channel<Boolean>){
        try
        {
            client.subscribe(topic, 0, null, object:IMqttActionListener {
                override fun onSuccess(asyncActionToken:IMqttToken) {
                    Log.w("Mqtt", "Subscribed on $topic")
                    CoroutineScope(IO).launch { async { channel.send(true) } }
                }
                override fun onFailure(asyncActionToken:IMqttToken, exception:Throwable) {
                    Log.w("Mqtt", "Subscription fail!")
                    CoroutineScope(IO).launch { async { channel.send(false) } }
                }
            })
        }
        catch (ex:MqttException) {
            System.err.println("Exception subscribing")
            ex.printStackTrace()
        }
    }

    // Unsubscribe the topic
    fun unsubscribe(topic: String){

        try
        {
            client.unsubscribe(topic,null,object :IMqttActionListener{
                override fun onSuccess(asyncActionToken: IMqttToken?) {
                }

                override fun onFailure(asyncActionToken: IMqttToken?, exception: Throwable?) {
                }

            })
        }
        catch (ex:MqttException) {
            System.err.println("Exception unsubscribe")
            ex.printStackTrace()
        }

    }

    fun publish(topic:String, message:String){
        try
        {
            client.publish(topic, message.toByteArray(),0,false,null,object :IMqttActionListener{
                override fun onSuccess(asyncActionToken: IMqttToken?) {
                    Log.w("Mqtt", "Publish Success!")
                }

                override fun onFailure(asyncActionToken: IMqttToken?, exception: Throwable?) {
                    Log.w("Mqtt", "Publish Failed!")
                }

            })
        }
        catch (ex:MqttException) {
            System.err.println("Exception publishing")
            ex.printStackTrace()
        }
    }

    @Synchronized fun id(context:Context):String {
        if (uniqueID == null)
        {
            val sharedPrefs = context.getSharedPreferences(
                PREF_UNIQUE_ID, Context.MODE_PRIVATE)
            uniqueID = sharedPrefs.getString(PREF_UNIQUE_ID, null)
            if (uniqueID == null)
            {
                uniqueID = UUID.randomUUID().toString()
                val editor = sharedPrefs.edit()
                editor.putString(PREF_UNIQUE_ID, uniqueID)
                editor.commit()
            }
        }
        return uniqueID!!
    }
}

data class MQTTConnectionParams(val clientId:String, val host: String, val username: String, val password: String){
}