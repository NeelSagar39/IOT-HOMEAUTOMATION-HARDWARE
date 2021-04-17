#include "WiFi.h"
#include "ESPAsyncWebServer.h"
 
#include <Servo.h>

static const int servoPin = 4;

Servo servo1;

const char* ssid = "HUAWEI-B535-B3DF";
const char* password =  "RM0N1DDGL92";
AsyncWebServer server(80);
int degree = 0;
int oldDegree = 0 ;
void setup(){
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());
  servo1.attach(servoPin);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
 
    int paramsNr = request->params();
    Serial.println(paramsNr);

    for(int i=0;i<paramsNr;i++){
 
        AsyncWebParameter* p = request->getParam(i);
        Serial.print("Param name: ");
        Serial.println(p->name());
        Serial.print("Param value: ");
        Serial.println(p->value());
        int Degree = (p->value()).toInt();
        degree = Degree;
        Serial.println("------");
    }
         reCalab();

    request->send(200, "text/plain", "message received");
  });
  server.begin();
}

void reCalab(){
  if (degree <=135 && degree >= 45){
  if (oldDegree != degree){
    if(degree < oldDegree){
        for(int posDegrees = oldDegree; posDegrees >= degree; posDegrees--) {
        servo1.write(posDegrees);
        Serial.println(posDegrees);
        delay(20);
        }
    }
    else{
              for(int posDegrees = oldDegree; posDegrees <= degree; posDegrees++) {
        servo1.write(posDegrees);
        Serial.println(posDegrees);
        delay(20);
        }
    }
        oldDegree = degree;
  }}
  else{
    degree = oldDegree;
    }
}


void loop() {}
