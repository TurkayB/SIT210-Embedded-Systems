#include <SPI.h>
#include <BH1750.h>
#include "DHT.h"
#include <Wire.h>
#include "secrets.h"
#include <WiFiNINA.h>
#include <PubSubClient.h>

#define DHTTYPE DHT22
#define DHTPIN 3
#define MOISTURE_PIN A0
#define MOISTURE_POWER 4

#define MQTT_SERVER "6a30d29d2b8944e09ae2653d6dcfaec2.s1.eu.hivemq.cloud"
#define MQTT_PORT 8883
#define MQTT_TOPIC "sensors_data"
#define MQTT_USER "hivemq.webclient.1748083588952"
#define MQTT_PASSWORD "7g5;68DvbfFTP.,@yHUd"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

BH1750 lightMeter;

DHT dht(DHTPIN, DHTTYPE);

WiFiSSLClient wifiSSLClient;
PubSubClient mqttClient(wifiSSLClient);

void connectMQTT() {
  while (!mqttClient.connected()) {
    Serial.println("Connecting to MQTT...");
    if (mqttClient.connect("ArduinoNanoIoT", MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("Connected to MQTT Broker!");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);

  //Connect to wifi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, pass);
  }
  Serial.println("Connected to WiFi");

  // Set MQTT Server
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  
  // Connect to MQTT
  connectMQTT();

  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.begin();

  lightMeter.begin();

  pinMode(MOISTURE_POWER, OUTPUT);

  dht.begin();

  Serial.println(F("Test begin"));
}

void loop() {
  //Keep wifi connection alive
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("Attempting to connect to SSID: ");
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  // Keep MQTT connection alive
  if (!mqttClient.connected()) {
    connectMQTT();
  }
  mqttClient.loop(); 

  //Print lux to serial monitor
  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");

  //Print soil moisture to serial monitor
  digitalWrite(MOISTURE_POWER, HIGH);
  delay(10);
  int moistureSensorValue = analogRead(MOISTURE_PIN);
  digitalWrite(MOISTURE_POWER, LOW);
  int moistureLevel = map(moistureSensorValue, 0, 1023, 0, 100); //Map to 0-100% range
  Serial.print("Moisture: ");
  Serial.print(moistureLevel);
  Serial.println("%");

  //Print temp and humidity to serial monitor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
  }
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%, Temperature: ");
  Serial.print(temperature);
  Serial.println(" Celsius");

  //Create payload with data to publish
  String payload = String(temperature) + "," + String(humidity) + "," + String(lux) + "," + String(moistureLevel);
  mqttClient.publish(MQTT_TOPIC, payload.c_str());

  delay(10000);
}