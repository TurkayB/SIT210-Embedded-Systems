#include <BH1750.h>
#include <Wire.h>
#include "secrets.h"
#include <WiFiNINA.h>
#include <PubSubClient.h>

#define MQTT_SERVER "6a30d29d2b8944e09ae2653d6dcfaec2.s1.eu.hivemq.cloud"
#define MQTT_PORT 8883
#define MQTT_TOPIC "light_sensor"
#define MQTT_USER "hivemq.webclient.1744602559447"
#define MQTT_PASSWORD "IBi@7uE1%*C4NJ3atfh#"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

BH1750 lightMeter;
bool msgSent = false;

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

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Set MQTT Server
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  
  // Connect to MQTT
  connectMQTT();

  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.begin();

  lightMeter.begin();

  Serial.println(F("BH1750 Test begin"));
}

void loop() {
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("Attempting to connect to SSID: ");
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  if (!mqttClient.connected()) {
    connectMQTT();
  }
  mqttClient.loop(); // Keep MQTT connection alive

  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");

  if (lux > 32000 && msgSent == false) {
    mqttClient.publish(MQTT_TOPIC, "Sunlight detected");
    msgSent = true;
  } else if (lux < 32000 && msgSent == true) {
    mqttClient.publish(MQTT_TOPIC, "No sunlight detected");
    msgSent = false;
  }
  delay(5000);
}