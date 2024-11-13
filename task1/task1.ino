#include <WiFi.h>
#include <PubSubClient.h>


#define red 4
#define grn 5
#define blu 22
#define ylw 19


const char* ssid = "Redmi 110A"; 
const char* pass = "99999999";   
const char* mqtt_broker = "mqtt.iotserver.uz"; 
const int mqtt_port = 1883; 
const char* mqtt_username = "userTTPU"; 
const char* mqtt_password = "mqttpass"; 

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void setup() {
  Serial.begin(115200);
  pinMode(red, OUTPUT);
  pinMode(grn, OUTPUT);
  pinMode(ylw, OUTPUT);
  pinMode(blu, OUTPUT);

 
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    delay(1000);
  }
  Serial.println("Connected to WiFi");
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(mqttCallback);

  connectToMqtt();
}

void loop() {
  
  if (!client.connected()) {
    connectToMqtt();
  }
  // Keep the MQTT connection alive
  client.loop();
}

void connectToMqtt(void) {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP32Client64654", mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT");
      client.subscribe("ttpu/control");
    } else {
      Serial.print("Failed to connect to MQTT, state: ");
      Serial.println(client.state());
      delay(3000);
    }
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Received message: ");
  Serial.println(message);
  Serial.print(" on topic: ");
  Serial.println(topic);

  if (message == "redon") {
    digitalWrite(red, HIGH);
    
  } else if (message == "redoff") {
    digitalWrite(red, LOW);
  }
  else if (message == "grnon"){
    digitalWrite(grn, HIGH);
    }
  else if (message == "grnoff"){
    digitalWrite(grn, LOW);
    }
  else if (message == "bluon"){
    digitalWrite(blu, HIGH);
    }
  else if (message == "bluoff"){
    digitalWrite(blu, LOW);
    }
  else if (message == "ylwon"){
    digitalWrite(ylw, HIGH);
    }
  else if (message == "ylwoff"){
    digitalWrite(ylw, LOW);
    }
  else {
    Serial.println("Unknown command.");
  }
}

