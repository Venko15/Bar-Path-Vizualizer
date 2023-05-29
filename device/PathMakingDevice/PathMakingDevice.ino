#include "InterfaceMPU.h"
#include "Configure.h"
#include "HighPassFilter.h"
#include "KalmanFilter.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#define X 0
#define Y 1
#define Z 2
#define ButtonPin 33

uint8_t btn_prev;
double  accX, accY, accZ;
double  posX = 0, posY = 0 , posZ=0;
double  velX = 0, velY = 0 , velZ = 0;

const char* ssid = "VIVACOM_NET";
const char* password = NULL;
const char* serverName = "http://192.168.1.12:3000/add_reading";
int tendency[3]= {0,0,0};

Config con;
InterfaceMPU &mpu = con.getMPU();
double x=0, p=1.0, q=0.1, r=1.0;
KalmanFilter kalmanX(x,p,q,r);
KalmanFilter kalmanY(x,p,q,r);
KalmanFilter kalmanZ(x,p,q,r);

HighPassFilter filter;

void setup() {
  
  HighPassFilter_init(&filter);
  WiFi.begin(ssid, NULL);
  Serial.begin(115200);
  while(!Serial);
  mpu.Init();

  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


  pinMode(ButtonPin, INPUT_PULLUP);
  
  btn_prev = digitalRead(ButtonPin);
}

double mabs(double val){
  return val<0?val*-1:val;
}

double accYArr[200];
double accXArr[200];
double accZArr[200];

double posXArr[200];
double posYArr[200];
double posZArr[200];
int i = 0; 

double realAccX=0, realAccY=0, realAccZ=0;
double dt = 0.2;
void loop() {
 uint8_t btn = digitalRead(ButtonPin);

 if (btn == LOW && btn_prev == HIGH)
  {
    unsigned long debounceTime = millis();

    while(i<200){

    if (digitalRead(ButtonPin) == LOW) {
      if ((millis() - debounceTime) >= 50) { // Button debouncing time
        break; // Exit the loop if the button is pressed
      }
    } else {
      debounceTime = millis();
    }

    mpu.getRealAcceleration(realAccX, realAccY, realAccZ, accX, accY, accZ);
    HighPassFilter_putX(&filter, realAccX );
    HighPassFilter_putY(&filter,realAccY );
    HighPassFilter_putZ(&filter, realAccZ);

    realAccX=HighPassFilter_getX(&filter);
    realAccY=HighPassFilter_getY(&filter);
    realAccZ=HighPassFilter_getZ(&filter);
    
    velX += ((realAccX) * dt);
    velY += ((realAccY) * dt);
    velZ += ((realAccZ) * dt);

    if(mabs(realAccX) < 0.1){
      velX=0;
    }
    if(mabs(realAccY) < 0.1){
      velY=0;
    }
    if(mabs(realAccZ) < 0.1){
      velZ=0;
    }
    posX += (velX)*dt;

    posY += (velY)*dt;

    posZ += (velZ)*dt;
    Serial.print(posY);
    Serial.print(" ");
    posX = kalmanX.update(posX);
    posY = kalmanY.update(posY);
    posZ = kalmanZ.update(posZ);
    double afterKalmanY = posY;
    Serial.println(afterKalmanY);
    posXArr[i] = posX;
    posYArr[i] = posY;
    posZArr[i] = posZ;

    i++;

    delay(200);
 

    }
    kalmanX.reset(x,p,q,r);
    kalmanY.reset(x,p,q,r);
    kalmanZ.reset(x,p,q,r);

    if(WiFi.status() == WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      http.begin(client, serverName);

      const size_t JSON_CAPACITY = 200 * JSON_ARRAY_SIZE(3); // Maximum capacity for 200 arrays with 3 elements each
      DynamicJsonDocument payload(JSON_CAPACITY);

      http.addHeader("Content-Type", "application/json");

      JsonArray payload_values = payload.createNestedArray("path");
      for (int j=0; j<i; j++) {
        JsonArray array = payload_values.createNestedArray();
        array.add(posXArr[j]);
        array.add(posYArr[j]);
        array.add(posZArr[j]);
      }
      String jsonString;
      serializeJson(payload, jsonString);
      Serial.println(jsonString);

      int res = http.POST(jsonString);
      Serial.println(res);
      http.end();
    }

      i=0;
    posX=0;
    posY=0;
    posZ=0;

    velX=0;
    velY=0;
    velZ=0;
  }

  btn_prev = digitalRead(ButtonPin);
  delay(500);
} 

