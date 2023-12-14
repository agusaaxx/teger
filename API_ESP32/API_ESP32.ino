#include <WiFi.h>
#include "DHTesp.h"
#include <HTTPClient.h>
#include <MQ135.h>



char ssid[] = "TP-Link_65GVk2";
char pass[] = "berakalah";
const char* serverName = "http://35.208.235.8:5000/api/v1/devices/data";
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;


#define DHTPIN 33
#define PIN_MQ135 34
#define sens 25
#define RLOAD 22.0
int val;
DHTesp dhtSensor;
MQ135 mq135_sensor(PIN_MQ135);

float temperature = 21.0; // Assume current temperature. Recommended to measure with DHT22
float humidity = 25.0; // Assume current humidity. Recommended to measure with DHT22
float h, t, f, hif, hic, chy;
String H,T,C;
String dataSend = "";

char myStr[50];
void setup() {
  Serial.begin(115200);
  pinMode(PIN_MQ135,INPUT);
  WiFi.begin(ssid, pass);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println(F("DHTxx test!"));

  dhtSensor.setup(DHTPIN, DHTesp::DHT11);
}

void loop() {
  TempAndHumidity  data = dhtSensor.getTempAndHumidity();
  float rzero = mq135_sensor.getRZero();
  float correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
  float resistance = mq135_sensor.getResistance();
  float ppm = mq135_sensor.getPPM();
  float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);
  chy = analogRead(sens);
  H=String(data.humidity);
  T=String(data.temperature);
  C=String(correctedPPM);
  dataSend = "{\"temperature\": " + (String)t + ", \"humidity\": " + (String)h + ", \"ldr\":\"0\",\"voltage\":\"0\",  \"sensor_node_id\":\"1\", \"soil_mosture\": " + (String)chy + "}";
  dataSend.toCharArray(myStr,50);
  //String data = String("{") + String("\");
  akuisisi();
  if ((millis() - lastTime) > timerDelay) {
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;
      http.begin(client, serverName);
      http.addHeader("Content-Type", "application/json");
      //int httpResponseCode = http.POST("{\"temperature\":\"0\",\"humidity\":\"0\",\"ldr\":\"0\",\"voltage\":\"0\",\"soilMosture\":\"0\"}");
      //int httpResponseCode = http.POST(data);
      //int httpResponseCode = http.POST("{\"ID\":\"01\",\"Value\":[\"{" + variable1 + "}\",\{" + variable2 + "}\"]}");
      //int httpResponseCode = http.POST(myStr);
      int httpResponseCode = http.POST("{\"temperature\": " + (String)t + ", \"humidity\": " + (String)h + ", \"ldr\":\"0\",\"voltage\":\"0\",  \"sensor_node_id\":\"1\", \"soil_mosture\": " + (String)chy + "}");
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}




void akuisisi() {

  TempAndHumidity  data = dhtSensor.getTempAndHumidity();
  float rzero = mq135_sensor.getRZero();
  float correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
  float resistance = mq135_sensor.getResistance();
  float ppm = mq135_sensor.getPPM();
  float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);

  // Serial.print("MQ135 RZero: ");
  // Serial.print(rzero);
  // Serial.print("\t Corrected RZero: ");
  // Serial.print(correctedRZero);
  // Serial.print("\t Resistance: ");
  // Serial.print(resistance);
  // Serial.print("\t PPM: ");
  // Serial.print(ppm);
  Serial.print("\t Hasil PPM: ");
  Serial.print(correctedPPM);
  Serial.print("ppm");
  Serial.print("  |  ");
  Serial.print(F("Humidity: "));
  Serial.print(data.humidity);
  Serial.print(F("%  Temperature: "));
  Serial.print(data.temperature);
  Serial.print(F("°C "));
  Serial.println("");
  // Serial.print("  |  ");
  // Serial.print("MQ135 RZero: ");
  // Serial.print("  |  ");
  // Serial.print(rzero);
  // Serial.print(hic);
  // Serial.print(F("°C "));
  // Serial.print(hif);
  // Serial.print(F("°F"));
  // Serial.print("     Soil :");
  // Serial.print(chy);
  // Serial.println("");
  delay(500);
}
