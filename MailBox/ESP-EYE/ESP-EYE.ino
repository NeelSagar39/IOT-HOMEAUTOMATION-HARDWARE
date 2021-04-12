/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-cam-post-image-photo-server/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
  
#include "WiFi.h"
#include <HTTPClient.h>
#include <Arduino.h>
#include <WiFi.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_camera.h"
#include <base64.h>

const char* ssid = "your_ssid";
const char* password = "your_pass";
String serverName = "Server_address";   // REPLACE WITH YOUR Raspberry Pi IP ADDRESS

const int serverPort = 8005;
int first_time = 1;
WiFiClient client;

// CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    4
#define SIOD_GPIO_NUM    18
#define SIOC_GPIO_NUM    23

#define Y9_GPIO_NUM      36
#define Y8_GPIO_NUM      37
#define Y7_GPIO_NUM      38
#define Y6_GPIO_NUM      39
#define Y5_GPIO_NUM      35
#define Y4_GPIO_NUM      14
#define Y3_GPIO_NUM      13
#define Y2_GPIO_NUM      34
#define VSYNC_GPIO_NUM   5
#define HREF_GPIO_NUM    27
#define PCLK_GPIO_NUM    25


const int timerInterval = 30000;    // time between each HTTP POST image
unsigned long previousMillis = 0;   // last time image was sent

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("ESP32-CAM IP Address: ");
  Serial.println(WiFi.localIP());
  
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // init with high specs to pre-allocate larger buffers
  if(psramFound()){
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 63;  //0-63 lower number means higher quality
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 63;  //0-63 lower number means higher quality
    config.fb_count = 1;
  }
  
  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    delay(1000);
    ESP.restart();
  }

  //sendPhoto(); 
}

void loop() {
  unsigned long currentMillis = millis();
  if (first_time == 1) {
    sendPhoto();
    previousMillis = currentMillis;
    first_time = 0;
  }
}

String sendPhoto() {
 String getAll;
  String getBody;

  camera_fb_t * fb = NULL;
  fb = esp_camera_fb_get();
  if(!fb) {
    Serial.println("Camera capture failed");
    delay(1000);
    ESP.restart();
  }
  Serial.println("image captured");
  Serial.println("Connecting to server: " + serverName);
  String encoded = base64::encode(fb->buf,fb->len);
  Serial.println(encoded);
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    String serverPath = serverName + "?id="+encoded;
    http.begin(serverPath.c_str());
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
    }}
///TRIAL CODE FOR SENDING IMAGE FILE INSTEAD BASE64 ENCODE
//  if (client.connect(serverName.c_str(), serverPort)) {
//    Serial.println("Connection successful!");    
//    String head = "--RandomNerdTutorials\r\nContent-Disposition: form-data; name=\"imageFile\"; filename=\"esp32-cam.jpeg\"\r\nContent-Type: image/jpeg\r\n\r\n";
//    String tail = "\r\n--RandomNerdTutorials--\r\n";
//
//    uint32_t imageLen = fb->len;
//    uint32_t extraLen = head.length() + tail.length();
//    uint32_t totalLen = imageLen + extraLen;
//  
//    client.println("POST " + serverPath + " HTTP/1.1");
//    client.println("Host: " + serverName);
//    client.println("Content-Length: " + String(totalLen));
//    client.println("Content-Type: multipart/form-data; boundary=RandomNerdTutorials");
//    client.println();
//    client.print(head);
//  
//    uint8_t *fbBuf = fb->buf;
//    size_t fbLen = fb->len;
////    for (size_t n=0; n<fbLen; n=n+1024) {
////      if (n+1024 < fbLen) {
////        client.write(fbBuf, 1024);
////        fbBuf += 1024;
////      }
////      else if (fbLen%1024>0) {
////        size_t remainder = fbLen%1024;
////        client.write(fbBuf, remainder);
////      }
////    }   
// client.write(fbBuf,imageLen);
//    client.print(tail);
//    
//    esp_camera_fb_return(fb);
//    
//    int timoutTimer = 10000;
//    long startTimer = millis();
//    boolean state = false;
//    
//    while ((startTimer + timoutTimer) > millis()) {
//      Serial.print(".");
//      delay(100);      
//      while (client.available()) {
//        char c = client.read();
//        if (c == '\n') {
//          if (getAll.length()==0) { state=true; }
//          getAll = "";
//        }
//        else if (c != '\r') { getAll += String(c); }
//        if (state==true) { getBody += String(c); }
//        startTimer = millis();
//      }
//      if (getBody.length()>0) { break; }
//    }
//    Serial.println();
//    client.stop();
//    Serial.println(getBody);
//  }
//  else {
//    getBody = "Connection to " + serverName +  " failed.";
//    Serial.println(getBody);
//  }
//  return getBody;
//}
