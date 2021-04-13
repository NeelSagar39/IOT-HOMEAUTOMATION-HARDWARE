#include <Stepper.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

const char* ssid = "HUAWEI-B535-B3DF";
const char* password =  "RM0N1DDGL92";

AsyncWebServer server(80);

const int stepsPerRevolution = 2038; // change this to fit the number of steps per revolution
 
// initialize the stepper library on pins 15,2,0,4
Stepper stepper(stepsPerRevolution, 13,12,14,27,4);
 
char* doorStatus = "closed";
 
void setup() {

 Serial.begin(115200);

 WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println(WiFi.localIP());

  server.on("/close", HTTP_PUT, [](AsyncWebServerRequest * request) {

      Serial.print("request received");
      if(doorStatus == "open"){
      stepper.setSpeed(60); // 1 rpm
      stepper.step(2038/4); // do 2038 steps -- corresponds to one revolution in one minute
      doorStatus = "closed";
      }
      request->send(200);
  });
 
  server.on("/open", HTTP_PUT, [](AsyncWebServerRequest * request) {

    if(doorStatus == "closed"){
    stepper.setSpeed(60); // 6 rpm
    stepper.step(-2038/4);
    doorStatus = "open";
    }
      request->send(200);
  });
 
  server.begin();

}
 
void loop() {}
