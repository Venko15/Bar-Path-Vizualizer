#include "InterfaceMPU.h"
#include "Configuire.h"
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
double  prevVelX = 0, prevVelY = 0 , prevVelZ = 0;
double prevAccX = 0, prevAccY = 0, prevAccZ = 0;
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
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");


  pinMode(ButtonPin, INPUT_PULLUP);
  
  btn_prev = digitalRead(ButtonPin);
}

double mod(double val){
  return val<0?val*-1:val;
}

double accYArr[200];
double accXArr[200];
double accZArr[200];

double posXArr[200];
double posYArr[200];
double posZArr[200];
int i = 0; 

void changeTendency(double value, double prevValue, int flag){//flag 0 ->x , 1->y, 2->z

  if(tendency[flag] == 0 && (value - prevValue) > 0){
    tendency[flag] = 1;
  }else if(tendency[flag] == 0 && (value - prevValue) < 0){
    tendency[flag] = -1;
  }
  if(mod(value - prevValue) < 0.1){
    tendency[flag] = 0;
  }
}

bool checkAdd(double value, double prevValue, int flag){
  if( ((value - prevValue) < 0  && tendency[flag] == -1 ) || ((value - prevValue) > 0  && tendency[flag] == 1 )  || tendency[flag] == 0){
    return true;
  }

  return false;
}
double realAccX=0, realAccY=0, realAccZ=0;

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


    prevAccX = realAccX;
    prevAccY = realAccY;
    prevAccZ = realAccZ;

    mpu.getRealAcceleration(realAccX, realAccY, realAccZ, accX, accY, accZ);
    HighPassFilter_putX(&filter, realAccX );
    HighPassFilter_putY(&filter,realAccY );
    HighPassFilter_putZ(&filter, realAccZ);

    realAccX=HighPassFilter_getX(&filter);
    realAccY=HighPassFilter_getY(&filter);
    realAccZ=HighPassFilter_getZ(&filter);
    
    velX += ((realAccX) * 0.2);
    velY += ((realAccY) * 0.2);
    velZ += ((realAccZ) * 0.2);

    if(mod(realAccX) < 0.1){
      velX=0;
    }
    if(mod(realAccY) < 0.1){
      velY=0;
    }
    if(mod(realAccZ) < 0.1){
      velZ=0;
    }
    posX += (velX)*0.2;

    posY += (velY)*0.2;

    posZ += (velZ)*0.2;

    Serial.println(posY*10);
    posX = kalmanX.update(posX);
    posY = kalmanY.update(posY);
    posZ = kalmanZ.update(posZ);

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

