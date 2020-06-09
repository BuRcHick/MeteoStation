#include <cstdint>
#include <vector>

#include "ArduinoJson.h"
#include "ESP8266WiFi.h"
#include "Thread.h"
#include "Wire.h"
#include "lib/isensor.hpp"
#include "lib/cbme200sensor_api.hpp"
#include "lib/mqttbroker_api.hpp"


#define HOSTNAME "MQTT_BROKER"

//------------------WIFI-SETTINGS-------------------------------------------------------------------
#define STASSID "SSID"           // Enter your access point ssid
#define STAPSK "PASSWORD"  // Enter access point password

//------------------MQTT-SETTINGS-------------------------------------------------------------------
#define MQTT_SERVER_PORT 1833

#define MQTT_CLINET_ID "HW"

// assign the ESP8266 pins to arduino pins
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12

#define BME_SCK D1
#define BME_MISO D5
#define BME_MOSI D2
#define BME1_CS D3
#define BME2_CS D4
#define BME3_CS D6

void onRecived(String topic, const char *data, uint32_t length);

Adafruit_BME280 g_bme1(BME1_CS, BME_MOSI, BME_MISO, BME_SCK);
Adafruit_BME280 g_bme2(BME2_CS, BME_MOSI, BME_MISO, BME_SCK);
Adafruit_BME280 g_bme3(BME3_CS, BME_MOSI, BME_MISO, BME_SCK);

CBME200TempSensor temp1(g_bme1);
CBME200HumSensor hum1(g_bme1);
CBME200PresSensor pres1(g_bme1);

CBME200TempSensor temp2(g_bme2);
CBME200HumSensor hum2(g_bme2);
CBME200PresSensor pres2(g_bme2);

CBME200TempSensor temp3(g_bme3);
CBME200HumSensor hum3(g_bme3);
CBME200PresSensor pres3(g_bme3);


CMQTTBroker g_MQTTBroker(onRecived);

static Thread s_mqttLoopThr = Thread();
static Thread s_sensorsMonitorThr = Thread();


ISensor* g_sensors[] = {
    &temp1, &hum1, &pres1,
    &temp2, &hum2, &pres2,
    &temp3, &hum3, &pres3};  // Array of sensors

const char* g_mqttSubscribeTopics[] = {"GET_SENSORS"};
const char* g_mqttPublishTopics[] = {"SESNORS_LIST", "SENSOR_UPDATE"};

static bool s_init = false;

void onRecived(String topic, const char *data, uint32_t length){
        if (!strcmp(topic.c_str(), "GET_SENSORS")) {
            s_init = false;
            Serial.println("getSensors");
            DynamicJsonDocument doc(1024);
            for (size_t i = 0; i < sizeof(g_sensors) / sizeof(g_sensors[0]); ++i) {
                JsonObject obj = doc.createNestedObject();
                obj[String("m_id")] = String(i);
                obj[String("m_type")] = sensorTypeToString(g_sensors[i]->getType());
                obj[String("m_status")] = String(g_sensors[i]->getStatus());
                obj[String("m_value")] = String(g_sensors[i]->getValue());
                String output;
                serializeJson(doc, output);
                Serial.println(output);
                g_MQTTBroker.publish("SESNORS_LIST", output.c_str(), output.length());
                doc.clear();
            }
            g_MQTTBroker.publish("LAST_SENSOR", nullptr, 0);
            s_init = true;
        }
}

static void s_sensorMonitor() {
    if(s_init) {
        Serial.println("s_sensorMonitor");
        for (size_t i = 0; i < sizeof(g_sensors) / sizeof(g_sensors[0]); ++i) {
            DynamicJsonDocument doc(2048);
            JsonObject obj = doc.createNestedObject();
            obj[String("m_id")] = String(i);
            obj[String("m_value")] = String(g_sensors[i]->getValue());
            String output;
            serializeJson(doc, output);
            g_MQTTBroker.publish("SENSOR_UPDATE", output.c_str(), output.length());
        }
    }
}

static void s_printValues() {
    for (int sensId = 0; sensId < sizeof(g_sensors) / sizeof(g_sensors[0]); ++sensId) {
        if (g_sensors[sensId]->getStatus() == eActive) {
            Serial.println("");
            Serial.print("{");
            Serial.print(sensId);
            Serial.print("} ");
            Serial.print(sensorTypeToString(g_sensors[sensId]->getType()));
            Serial.print(" {");
            Serial.print(g_sensors[sensId]->getValue());
            Serial.print("} ");
        }
    }
}

static void s_wifiSetup(const char* ssid, const char* password) {
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA);
    WiFi.hostname(HOSTNAME);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void setup() {
    Serial.begin(115200);
    s_wifiSetup(STASSID, STAPSK);
    g_MQTTBroker.init();
    for (size_t i = 0; i < sizeof(g_mqttSubscribeTopics)/sizeof(g_mqttSubscribeTopics[0]); ++i) {
        g_MQTTBroker.subscribe(g_mqttSubscribeTopics[i]);
    }
}

void loop() {
    s_sensorMonitor();
    delay(1000);
}