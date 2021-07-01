#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#else
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;
#endif

#include <MQTT.h>

const char ssid1[] = "RED-111";
const char pass1[] = "74241767ab";
const char ssid2[] = "TPS";
const char pass2[] = "3DLAB2019";
const char ssid3[] = "Casa_EXT";
const char pass3[] = "BASCULA2021";

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;
void Conectar() {
  Serial.print("Conectando a Wifi...");
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.print("\nConectado a MQTT...");

  while (!client.connect("Banda Transportadora", "banda2021",  "BANDA12345")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nConectado MQTT!");

  client.subscribe("BANDA");
}
#define posicion_mandarina 0
#define posicion_uva_verde 120
#define posicion_uva_roja 240
int GradoActual = 0;
int GradoDeseado = 0;

const int BotonIzquierda = 16;
const int BotonDerecha = 17;

const int P_Step = 19;
const int P_Dir = 18;
const int led_estado = 13;
const int led_mandarina = 12;
const int led_uva_verde = 14;
const int led_uva_roja = 27;


void setup() {
  pinMode(P_Step, OUTPUT);
  pinMode(P_Dir, OUTPUT);
  pinMode(led_estado, OUTPUT);
  pinMode(led_mandarina, OUTPUT);
  pinMode(led_uva_verde, OUTPUT);
  pinMode(led_uva_roja, OUTPUT);
  pinMode(BotonIzquierda, INPUT);
  pinMode(BotonDerecha, INPUT);
  Serial.begin(9600);
  Serial.println("Iniciando Wifi");
  WiFi.mode(WIFI_STA);//Cambiar modo del Wi-Fi
  delay(100);
  wifiMulti.addAP("RED-111", "74241767ab");
  wifiMulti.addAP("TPS", "3DLAB2019");
  wifiMulti.addAP("Casa_EXT","BASCULA2021");

  client.begin("banda2021.cloud.shiftr.io", net);
  client.onMessage(RecibirMQTT);

  Conectar();
}

void loop() {
  client.loop();
  delay(10);
  if (!client.connected()) {
   Conectar();
  }
  ActualizarPosicionMotores();
  ActualizarBotones();
}

void ActualizarPosicionMotores() {
  Serial.print(GradoActual);
  Serial.print("-");
  Serial.print(GradoDeseado);
  Serial.println();
  if (GradoActual < GradoDeseado) {
    SubirGrado();
  }
  else if ( GradoActual > GradoDeseado) {
    BajarGrado();
  }
}

void SubirGrado() {
  Serial.println("Moviendo Abajo");
  GradoActual++;
  MoverMotor(1);
}

void BajarGrado() {
  Serial.println("Moviendo Abajo");
  GradoActual--;
  MoverMotor(-1);
}

void MoverMotor(int Pasos) {
  if (Pasos > 0) {
    digitalWrite(P_Dir, HIGH);
  } else {
    digitalWrite(P_Dir, LOW);
    Pasos = -Pasos;
  }
  for (int i = 0; i < Pasos; i++) {
    digitalWrite(P_Step, HIGH);
    delay(10);
    digitalWrite(P_Step, LOW);
    delay(10);
  }
}

void ActualizarBotones() {
  if (digitalRead(BotonIzquierda)) {
    Serial.println("Reiniciando hacia la izquierda");
    MoverMotor(1);
    GradoActual = 0;
    GradoDeseado = 0;
    digitalWrite(led_mandarina, LOW);
    digitalWrite(led_uva_verde, LOW);
    digitalWrite(led_uva_roja, LOW);
  }
  if (digitalRead(BotonDerecha)) {
    Serial.println("Reiniciando hacia la derecha");
    MoverMotor(-1);
    GradoActual = 0;
    GradoDeseado = 0;
    digitalWrite(led_mandarina, LOW);
    digitalWrite(led_uva_verde, LOW);
    digitalWrite(led_uva_roja, LOW);
  }
}
void RecibirMQTT(String &topic, String &payload) {
  Serial.println("Recibio: " + topic + " - " + payload);
  if (payload == "MANDARINA") {
    GradoDeseado = posicion_mandarina;
    Serial.println("Detecta_Mandarina");
    digitalWrite(led_mandarina, HIGH);
    digitalWrite(led_uva_roja, LOW);
    digitalWrite(led_uva_verde, LOW);
  } else if (payload == "UVA ROJA") {
    GradoDeseado = posicion_uva_roja;
    Serial.println("Detecta_uva_roja");
    digitalWrite(led_mandarina, LOW);
    digitalWrite(led_uva_verde, LOW);
    digitalWrite(led_uva_roja, HIGH);
  }
  else if (payload == "UVA VERDE") {
   GradoDeseado = posicion_uva_verde;
   Serial.println("Detectar_uva_verde");
   digitalWrite(led_mandarina, LOW);
   digitalWrite(led_uva_verde, HIGH);
   digitalWrite(led_uva_roja, LOW);
 }
}
