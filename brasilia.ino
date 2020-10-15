/*
*/
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#ifndef STASSID
#define STASSID "your_wifi"
#define STAPSK  "your_wifi_password"
#endif
const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "api.openweathermap.org"; //waar die heen moet kan ook google.com zijn
const uint16_t port = 80; //http:80 https:443
//const int led = 2;
Servo myservo;  // create servo object to control a servo
ESP8266WiFiMulti WiFiMulti;

int pos = 0;    // variable to store the servo position
void setup() {
  myservo.attach(2);  // attaches the servo on GPIO2 to the servo object
  Serial.begin(115200);

  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);


  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(500);
  }

  pos = 0; //TEST
  myservo.write(pos);  // tell servo to go to position in variable 'pos'
  delay(500);
  pos = 100;
  myservo.write(pos);  // tell servo to go to position in variable 'pos'

  delay(500);

}


void loop() {
  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  if (!client.connect(host, port)) {
    delay(5000);
    return;
  } else {
    client.println("GET /data/2.5/weather?q=brasilia&APPID=67f4a76fce6a321e237cd63a2a19db8e"); //if clint is connected get api
    client.println();

  }

  //read back one line from server
  String response = client.readStringUntil('\r'); //read response
  if (response.indexOf("rain") > 0) { //if there's a rain in there
    pos = 0; //let it rain
    myservo.write(pos);  // tell servo to go to position in variable 'pos'
    delay(1200000); // open for 20 min
    pos = 100;
    myservo.write(pos);  // tell servo to go to position in variable 'pos'
  } else {
    Serial.println("dry");
  }
  client.stop();
  delay(3600000); //check again in 60 min
}
