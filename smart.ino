

// Weather application with Firebase and MIT App Inventor



#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// set the firebase host and auth. and wifi settings
#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

#include <DHTesp.h>

DHTesp dht;

void setup() { 
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
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); //log in to firebase
  
}

void loop() {
  int hum = dht.getHumidity();
  int tem = dht.getTemperature();
  // Get data fron dht sensor
  Serial.print("hum: "); Serial.println(hum);
  Serial.print("tem: "); Serial.println(tem);
  // Send data to firebase
  Firebase.setInt("hum",hum);
  if (Firebase.failed()) {
      Serial.print("setting /hum failed:");
      Serial.println(Firebase.error());  
  }
  
  Firebase.setInt("tem",tem);
  if (Firebase.failed()) {
      Serial.print("setting /tem failed:");
      Serial.println(Firebase.error());  
  }
  // set the lamp statue 
  String lampStatus = Firebase.getString("lamp");
  Serial.print("lampStatus"); Serial.println(lampStatus);

  if(lampStatus == "1"){
    digitalWrite(D7, LOW);
  }
  else if(lampStatus == "0"){
    digitalWrite(D7, HIGH);
    
  }
  

}
