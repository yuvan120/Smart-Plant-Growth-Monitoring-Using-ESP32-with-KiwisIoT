#include <WiFi.h>

#include <KiwisIoT.h>

#include <DHT.h>

// WiFi credentials

const char* ssid = "YOUR_WIFI_NAME";

const char* password = "YOUR_WIFI_PASSWORD";

// KiwisIoT panel topic

const char* topic = "YOUR_DASHBOARD_TOPIC_ID";

// Sensor pins

#define SOIL_PIN 34

#define LDR_PIN 35

#define DHT_PIN 4

#define DHTTYPE DHT22

DHT dht(DHT_PIN, DHTTYPE);

KiwisIoT kiwisiot(ssid, password, topic);

// Calibrate these two values for your sensor

int dryValue = 3000;

int wetValue = 1200;

void setup() {

Serial.begin(115200);

dht.begin();

kiwisiot.begin();

Serial.println("Smart Plant Monitoring Started");

}

void loop() {

// Read soil moisture

int soilRaw = analogRead(SOIL_PIN);

int soilPercent = map(

soilRaw, dryValue, wetValue, 0, 100

);

soilPercent = constrain(soilPercent, 0, 100);

// Read temperature and humidity

float temperature = dht.readTemperature();

float humidity = dht.readHumidity();

// Read light sensor

int lightRaw = analogRead(LDR_PIN);

int lightPercent = map(lightRaw, 0, 4095, 0, 100);

lightPercent = constrain(lightPercent, 0, 100);

if (isnan(temperature) || isnan(humidity)) {

Serial.println("DHT sensor error");

delay(2000);

return;

}

// Plant condition

String status;

if (soilPercent < 30) {

status = "Needs Water";

}

else if (temperature > 35) {

status = "High Temperature";

}

else if (lightPercent < 20) {

status = "Low Light";

}

else {

status = "Healthy Conditions";

}

// Send data to KiwisIoT

kiwisiot.send("0", soilPercent);

kiwisiot.send("1", temperature);

kiwisiot.send("2", humidity);

kiwisiot.send("3", lightPercent);

kiwisiot.send("4", status);

kiwisiot.run();

// Serial monitor

Serial.println("---------------------");

Serial.print("Soil: ");

Serial.println(soilPercent);

Serial.print("Temperature: ");

Serial.println(temperature);

Serial.print("Humidity: ");

Serial.println(humidity);

Serial.print("Light: ");

Serial.println(lightPercent);

Serial.print("Plant Status: ");

Serial.println(status);

delay(5000);

}