#include <vector>
#include <cstdint>
#include "ESP8266WiFi.h"
#include "PubSubClient.h"
#include "Adafruit_BME280.h"
#include "Adafruit_Sensor.h"
#include "Wire.h"
#include "SPI.h"
#include "lib/csensor_api.hpp"


//------------------WIFI-SETTINGS-------------------------------------------------------------------
#define STASSID "Pentagon"  //Enter your access point ssid
#define STAPSK  "venividivici228_JP"      //Enter access point password

//------------------MQTT-SETTINGS-------------------------------------------------------------------
#define MQTT_SERVER "localhost"
#define MQTT_SERVER_PORT 1833

//------------------BME-SETTINGS-------------------------------------------------------------------
#define SEALEVELPRESSURE_HPA (1013.25)
#define SENSORS_COUNT 9 //Enter your sensors count
#define VALUE_PRESCALLER 100

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

Adafruit_BME280 g_bme1(BME1_CS, BME_MOSI, BME_MISO, BME_SCK);
Adafruit_BME280 g_bme2(BME2_CS, BME_MOSI, BME_MISO, BME_SCK);
Adafruit_BME280 g_bme3(BME3_CS, BME_MOSI, BME_MISO, BME_SCK);

CSensor g_sensors[SENSORS_COUNT]; //Vector of sensors

void s_getBme1Temperature(int& value){
  value = (int)(g_bme1.readTemperature() * (VALUE_PRESCALLER));
}
void s_getBme1Humidity(int& value){
  value = (int)(g_bme1.readHumidity() * (VALUE_PRESCALLER));
}
void s_getBme1Pressure(int& value){
  value = (int)(g_bme1.readPressure() * (VALUE_PRESCALLER));
}

void s_getBme2Temperature(int& value){
  value = (int)(g_bme2.readTemperature() * (VALUE_PRESCALLER));
}
void s_getBme2Humidity(int& value){
  value = (int)(g_bme2.readHumidity() * (VALUE_PRESCALLER));
}
void s_getBme2Pressure(int& value){
  value = (int)(g_bme2.readPressure() * (VALUE_PRESCALLER));
}

void s_getBme3Temperature(int& value){
  value = (int)(g_bme3.readTemperature() * (VALUE_PRESCALLER));
}
void s_getBme3Humidity(int& value){
  value = (int)(g_bme3.readHumidity() * (VALUE_PRESCALLER));
}
void s_getBme3Pressure(int& value){
  value = (int)(g_bme3.readPressure() * (VALUE_PRESCALLER));
}

static void s_printValues() {
    for (int sensId = 0; sensId < SENSORS_COUNT; ++sensId) {
        if(g_sensors[sensId].getStatus() == active_state) {       
            Serial.println("");
            Serial.print("{");
            Serial.print(g_sensors[sensId].getId());
            Serial.print("} ");
            Serial.print(sensorTypeToString(g_sensors[sensId].getType()));
            Serial.print(" {");
            Serial.print(g_sensors[sensId].getValue());
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

static void s_sensorsConfiguration() {
    g_sensors[0].setType(temperature);
    g_sensors[0].setCallback(s_getBme1Temperature);
    g_sensors[0].setStatus((sensorStatus)g_bme1.begin());
    g_sensors[1].setType(humidity);
    g_sensors[1].setCallback(s_getBme1Humidity);
    g_sensors[1].setStatus((sensorStatus)g_bme1.begin());
    g_sensors[2].setType(pressure);
    g_sensors[2].setCallback(s_getBme1Pressure);
    g_sensors[2].setStatus((sensorStatus)g_bme1.begin());

    g_sensors[3].setType(temperature);
    g_sensors[3].setCallback(s_getBme2Temperature);
    g_sensors[3].setStatus((sensorStatus)g_bme2.begin());
    g_sensors[4].setType(humidity);
    g_sensors[4].setCallback(s_getBme2Humidity);
    g_sensors[4].setStatus((sensorStatus)g_bme2.begin());
    g_sensors[5].setType(pressure);
    g_sensors[5].setCallback(s_getBme2Pressure);
    g_sensors[5].setStatus((sensorStatus)g_bme2.begin());

    g_sensors[6].setType(temperature);
    g_sensors[6].setCallback(s_getBme3Temperature);
    g_sensors[6].setStatus((sensorStatus)g_bme3.begin());
    g_sensors[7].setType(humidity);
    g_sensors[7].setCallback(s_getBme3Humidity);
    g_sensors[7].setStatus((sensorStatus)g_bme3.begin());
    g_sensors[8].setType(pressure);
    g_sensors[8].setCallback(s_getBme3Pressure);
    g_sensors[8].setStatus((sensorStatus)g_bme3.begin());
}

void setup() {
    Serial.begin(115200);
    s_wifiSetup(STASSID, STAPSK);
    s_sensorsConfiguration();
}

void loop() {
    Serial.print("connecting to ");
    Serial.print(MQTT_SERVER);
    Serial.print(':');
    Serial.println(MQTT_SERVER_PORT);
    WiFiClient wifiClient;
    PubSubClient client(MQTT_SERVER, MQTT_SERVER_PORT, wifiClient); // 1883 is the listener port for the Broker
    s_printValues();
    delay(1000);
}
