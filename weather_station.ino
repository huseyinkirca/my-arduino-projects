
// hava durumu merkezi olarak adlandırdığım IOT projem 

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <DHTesp.h>
#include "AnotherIFTTTWebhook.h"

DHTesp dht;

#define WLAN_SSID       ""             // MODEM ADI
#define WLAN_PASS       ""        // MODEM ŞİFRESİ 

/************************* Adafruit.io Setup *********************************/
#define IFTTT_Key "h_KRVu3mU_hFoH6fG4_6zE20unGEWjHCfN3YANcWG37"
#define IFTTT_Event "water_sensor"
#define AIO_SERVER      "io.adafruit.com" //Adafruit Server
#define AIO_SERVERPORT  1883                   
#define AIO_USERNAME    "huseyinkirca"            // ADAFRUİT.İO KULLANICI ADI
#define AIO_KEY         "aio_GRbb66ePmaCDlSO1MqaMNU2pMefo"   // ADAFRUİT.İO ANAHTAR KODU


int result = A0;
int water_level = 0;
int notify = 0;
int notify_chc = 0;
//WIFI CLIENT
WiFiClient client1;
Adafruit_MQTT_Client mqtt(&client1, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temp"); // verileri Adafruite gönderdiğim kısım
Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/hum");
Adafruit_MQTT_Publish water_sen = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/water");

void MQTT_connect();

void setup() {
  Serial.begin(115200);

  dht.setup(D8,DHTesp::DHT11);
  
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
  send_webhook(IFTTT_Event,IFTTT_Key,"test","TEST","Test");
 
}

void loop() {
 
  MQTT_connect();

  float h = dht.getHumidity();
  // Read temperature as Celsius
  float t = dht.getTemperature();
  //publish temperature and humidity
  water_level = analogRead(result);
  
  Serial.print(F("\nTemperature: "));
  Serial.print(t);
  Serial.print(F("\nHumidity: "));
  Serial.print(h);
  Serial.print(F("\nWater Level: "));
  Serial.print(water_level);
  
  temperature.publish(t);
  humidity.publish(h);
  water_sen.publish(water_level);

  if (water_level > 85){
    notify++;
    if (notify > 10){
      notify_chc++;
      if (notify_chc > 50){
        notify_chc = 0 ;
        notify = 0;
      }
    }else{
      send_webhook(IFTTT_Event,IFTTT_Key,"Galiba","yagmur","yagıyor"); // yağmur algıladığında telefona bildirim gönderiyor
    }
    
  }
  delay(10000);
 
}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000); 
    retries--;
    if (retries == 0) {
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
  
}
