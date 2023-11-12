#include <WiFi.h>
#include <ArduinoJson.h>
#include <DHTesp.h>
#include <PubSubClient.h>

// Configurações de WiFi
const char *SSID = "Wokwi-GUEST";
const char *PASSWORD = "";  // Substitua pelo sua senha

// Configurações de MQTT
const char *BROKER_MQTT = "broker.hivemq.com";
const int BROKER_PORT = 1883;
const char *ID_MQTT = "mafe_mqtt";
const char *TOPIC_PUBLISH_TEMP_HUMI = "FIAP/CP/TempHumi";
const char *TOPIC_PUBLISH_DISTANCE = "FIAP/CP/Distance";


// Configurações de Hardware
#define PIN_DHT 12
#define RED_LED 4
#define YELLOW_LED 2
#define GREEN_LED 15
#define ECHO_PIN 21
#define TRIG_PIN 19
#define PUBLISH_DELAY 2000
 

// Variáveis globais
WiFiClient espClient;
PubSubClient MQTT(espClient);
DHTesp dht;
unsigned long publishUpdate = 0;
TempAndHumidity sensorValues;
const int TAMANHO = 200;


// Protótipos de funções
void updateSensorValues();
void initWiFi();
void initMQTT();
void callbackMQTT(char *topic, byte *payload, unsigned int length);
void reconnectMQTT();
void reconnectWiFi();
void checkWiFIAndMQTT();

void updateSensorValues() {
  sensorValues = dht.getTempAndHumidity();
}

void initWiFi() {
  Serial.print("Conectando com a rede: ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Conectado com sucesso: ");
  Serial.println(SSID);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(callbackMQTT);
}

void callbackMQTT(char *topic, byte *payload, unsigned int length) {
  String msg = String((char*)payload).substring(0, length);
  
  Serial.printf("Mensagem recebida via MQTT: %s do tópico: %s\n", msg.c_str(), topic);

  
}


void reconnectMQTT() {
  while (!MQTT.connected()) {
    Serial.print("Tentando conectar com o Broker MQTT: ");
    Serial.println(BROKER_MQTT);

    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado ao broker MQTT!");
      //entrar no mqtt
      
      
    } else {
      Serial.println("Falha na conexão com MQTT. Tentando novamente em 2 segundos.");
      delay(2000);
    }
  }
}

void checkWiFIAndMQTT() {
  if (WiFi.status() != WL_CONNECTED) reconnectWiFi();
  if (!MQTT.connected()) reconnectMQTT();
}

void reconnectWiFi(void) {
  if (WiFi.status() == WL_CONNECTED)
    return;

  WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Wifi conectado com sucesso");
  Serial.print(SSID);
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);

  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  

  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  
  dht.setup(PIN_DHT, DHTesp::DHT22);
  
  
  
  
  
  initWiFi();
  initMQTT();
}

float readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  int duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2;
}

void loop() {
  checkWiFIAndMQTT();
  MQTT.loop();

  float distance = readDistanceCM();


  //Verificação de distancia
  if(distance < 100){

    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
  }else{
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
  }

  //Verificação da temperatura
  
  if((sensorValues.temperature > 25) || (sensorValues.temperature < 15)){

    digitalWrite(YELLOW_LED, HIGH);
    
  }else{
    digitalWrite(YELLOW_LED, LOW);
  }

  

  delay(2000);

  if ((millis() - publishUpdate) >= PUBLISH_DELAY) {
    publishUpdate = millis();
    updateSensorValues();
    readDistanceCM();

    if (!isnan(sensorValues.temperature) && !isnan(sensorValues.humidity)) {
      StaticJsonDocument<TAMANHO> doc;
      doc["temperatura"] = sensorValues.temperature;
      doc["umidade"] = sensorValues.humidity;

      char buffer[TAMANHO];
      serializeJson(doc, buffer);
      MQTT.publish(TOPIC_PUBLISH_TEMP_HUMI, buffer);
      Serial.println(buffer);
    }
    if (!isnan(distance)) {
      StaticJsonDocument<TAMANHO> doc;
      doc["distance"] = readDistanceCM();
      

      char buffer[TAMANHO];
      serializeJson(doc, buffer);
      MQTT.publish(TOPIC_PUBLISH_DISTANCE, buffer);
      Serial.println(buffer);
    }
  }

  
}
