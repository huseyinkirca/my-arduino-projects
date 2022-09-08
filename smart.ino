

// Firebase, MIT App Inventor ile yaptığım IOT projem



#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// firebase ve wifi değişkenleri ayarlama
#define FIREBASE_HOST "node-7383d-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "ZalHL07uBR1KYseWRxxgawlCbXksMHIhLzBC3zU9"
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

#include <DHTesp.h>

DHTesp dht;

void setup() { //firebase ve wifi ye balantı kurma kısmı
  Serial.begin(115200);
  dht.setup(D4,DHTesp::DHT11);
  pinMode(D7,OUTPUT);
  digitalWrite(D7, HIGH);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
}

void loop() {
  int hum = dht.getHumidity();
  int tem = dht.getTemperature();
  // sensörden veri alma
  Serial.print("hum: "); Serial.println(hum);
  Serial.print("tem: "); Serial.println(tem);
  // verileri firebase e gönderme
  Firebase.setInt("hum",hum);
  if (Firebase.failed()) {
      Serial.print("setting /hum failed:");
      Serial.println(Firebase.error());  
  }
  // firebasedeki değişimlere göre işlem yürütüldüğü kısım
  Firebase.setInt("tem",tem);
  if (Firebase.failed()) {
      Serial.print("setting /tem failed:");
      Serial.println(Firebase.error());  
  }
  
  String lampStatus = Firebase.getString("lamp");
  Serial.print("lampStatus"); Serial.println(lampStatus);

  if(lampStatus == "1"){
    digitalWrite(D7, LOW);
  }
  else if(lampStatus == "0"){
    digitalWrite(D7, HIGH);
    
  }
  

}
