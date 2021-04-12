#include "WiFi.h"
#include <HTTPClient.h>
#include <Servo.h>

static const int servoPin = 13;
Servo servo1;

const char* ssid = "realme C2";
const char* password =  "die_soon";

String serverName = "https://outsidehomeautomation.herokuapp.com/carparking";

unsigned long lastTime = 0;
unsigned long timerDelay = 500;

int flag1=0;
int flag2=0;

const int trigPin = 2;
const int echoPin = 0;

long duration;
int distance;


void setup() {
 
  Serial.begin(115200);
  servo1.attach(servoPin);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
  
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
 
}
 
void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  Serial.print("Dustance: ");
  Serial.println(distance);

  
  if (distance<=6 && flag2==0) {
    flag2=1;
    flag1=0;
    servo1.write(90);
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      
      String serverPath = serverName + "?id="+distance;
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }

  if (distance>30 && flag1==0) {
    flag1=1;
    flag2=0;
    servo1.write(0);
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      
      String serverPath = serverName + "?id="+distance;
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
  
  if ((millis() - lastTime) > timerDelay && distance<=30 && distance>6) {
    //Check WiFi connection status
    flag1=0;
    flag2=0;
    servo1.write(0);
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      
      String serverPath = serverName + "?id="+distance;
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
  }
