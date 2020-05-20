#include <vector>
#include <cstdint>
#include "Adafruit_BME280.h"
#include "Adafruit_Sensor.h"
#include "Wire.h"
#include <SPI.h>

//------------------BME-SETTINGS-------------------------------------------------------------------
#define SEALEVELPRESSURE_HPA (1013.25)
#define SENSORS_COUNT 3//Enter your sensors count
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


#define SEALEVELPRESSURE_HPA (1013.25)

typedef enum {
    no_respond,
    active_state,
}sensor_status_t;

struct mySensor_t{
    uint8_t id;
    long int tempValue;
    long int humValue;
    long int presValue;
    Adafruit_BME280* sensorInst;
    sensor_status_t sensorStatus;
};


Adafruit_BME280 bme1(BME1_CS, BME_MOSI, BME_MISO, BME_SCK);
Adafruit_BME280 bme2(BME2_CS, BME_MOSI, BME_MISO, BME_SCK);
Adafruit_BME280 bme3(BME3_CS, BME_MOSI, BME_MISO, BME_SCK);


static Adafruit_BME280* g_bmeRef[] = {&bme1, &bme2, &bme3}; //Vector of sensors
static mySensor_t g_sensors[SENSORS_COUNT]; //Vector of sensors

static void s_monitorValues() {
    for (int sensId = 0; sensId < SENSORS_COUNT; ++sensId) {
        if(g_sensors[sensId].sensorStatus == active_state) { 
            g_sensors[sensId].tempValue = (long int)(g_sensors[sensId].sensorInst->readTemperature() * (VALUE_PRESCALLER));
            g_sensors[sensId].humValue = (long int)(g_sensors[sensId].sensorInst->readHumidity() * (VALUE_PRESCALLER));
            g_sensors[sensId].presValue = (long int)(g_sensors[sensId].sensorInst->readPressure());
        }
    }
}

static void s_printValues() {
    for (int sensId = 0; sensId < SENSORS_COUNT; ++sensId) {
        if(g_sensors[sensId].sensorStatus == active_state) {       
            Serial.println("");
            Serial.print("{");
            Serial.print(g_sensors[sensId].id);
            Serial.print("} ");
            Serial.print("Temperature");
            Serial.print("{");
            Serial.print(g_sensors[sensId].tempValue);
            Serial.print("} ");
            Serial.print("Humidity");
            Serial.print("{");
            Serial.print(g_sensors[sensId].humValue);
            Serial.print("} ");
            Serial.print("Pressure");
            Serial.print("{");
            Serial.print(g_sensors[sensId].presValue);
            Serial.print("} ");
        }
    }
}

void setup() {
    Serial.begin(115200);
        
    //------------------------------SENSORS-CONFIGURATION---------------------------------------------------
    for (size_t i = 0; i < SENSORS_COUNT; i++) {
        g_sensors[i].id = i;
        g_sensors[i].sensorInst = g_bmeRef[i];
        g_sensors[i].sensorStatus = (sensor_status_t)(g_sensors[i].sensorInst->begin());
    }
}

void loop() {
    s_monitorValues();
    s_printValues();
    delay(1000);
}
