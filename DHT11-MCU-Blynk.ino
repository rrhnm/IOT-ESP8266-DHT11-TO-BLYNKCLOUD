#define BLYNK_TEMPLATE_ID "TMPL68k1SXIUH"
#define BLYNK_TEMPLATE_NAME "MCUToDHT11"
#define BLYNK_AUTH_TOKEN "Zsypdp7_gS2NH0gEMNpqSrTaUzrFjFNc"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "IoTHan";
char pass[] = "mantapbang";

#define DHTPIN D2        
#define DHTTYPE DHT11


#define LED_Merah D5   
#define LED_Putih D6   
#define LED_Kuning D7

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

float maxTemp = 30.0;  
float minTemp = 20.0;  
float maxHum = 70.0;   
float minHum = 30.0;   

void setup(){
  pinMode(LED_Merah, OUTPUT);
  pinMode(LED_Putih, OUTPUT);
  pinMode(LED_Kuning, OUTPUT);
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  dht.begin();
  timer.setInterval(5000L, sendSensor);
}

void loop(){
  Blynk.run();
  timer.run();
}

void sendSensor(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();  
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V0, t); 
  Blynk.virtualWrite(V1, h);

  if (t > maxTemp) {
    digitalWrite(LED_Merah, HIGH);
    Blynk.virtualWrite(V2, "Warning: Suhu terlalu tinggi!"); 
    Blynk.logEvent("Temperatur", "> 30*C (Tinggi)");  
  } else if (t < minTemp) {
    digitalWrite(LED_Kuning, HIGH);
    Blynk.virtualWrite(V2, "Warning: Suhu terlalu rendah!");
    Blynk.logEvent("Temperatur", "> 20*C (Rendah)");  
  } else {
    digitalWrite(LED_Merah, LOW);
    digitalWrite(LED_Kuning, LOW); 
  }

  if (h > maxHum) {
    digitalWrite(LED_Putih, HIGH); 
    Blynk.virtualWrite(V3, "Warning: Kelembapan terlalu tinggi!");
    Blynk.logEvent("Kelembapan", "> 70% (Tinggi)");  
  } else if (h < minHum) {
    digitalWrite(LED_Kuning, HIGH); 
    Blynk.virtualWrite(V3, "Warning: Kelembapan terlalu rendah!");  
    Blynk.logEvent("Kelembapan", "< 30% (Rendah)");  
  } else {
    digitalWrite(LED_Putih, LOW);  
    digitalWrite(LED_Kuning, LOW);
  }

  Serial.print("Suhu : ");
  Serial.print(t);
  Serial.print(" || Kelembapan : ");
  Serial.println(h);
}