#include "InterfaceMPU.h"
#include "Configuire.h"
#include "HighPassFilter.h"
using namespace std;

double  accX, accY, accZ;
double  posX = 0, posY = 0 , posZ=0;
double  velX = 0, velY = 0 , velZ = 0;
double  prevVelX = 0, prevVelY = 0 , prevVelZ = 0;
double prevAccX = 0, prevAccY = 0, prevAccZ = 0;
int tendencyX = 0;
int tendencyY = 0;
int tendencyZ = 0;
Config con;
InterfaceMPU &mpu = con.getMPU();

HighPassFilter filter;

void setup() {
  HighPassFilter_init(&filter);
  Serial.begin(115200);
  while(!Serial);
  mpu.Init();
}

double mod(double val){
  return val<0?val*-1:val;
}
double posYArr[200];
double accYArr[200];
int i = 0; 

void changeTendency(double value, double prevValue, int flag){//flag 0 ->x , 1->y, 2->z

}
  double realAccX=0, realAccY=0, realAccZ=0;

void loop() {

  while(i<200){

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

  if(tendencyY == 0 && (realAccY - prevAccY) > 0){
    tendencyY = 1;
  }else if(tendencyY == 0 && realAccY - prevAccY < 0){
    tendencyY = -1;
  }
  if(mod(realAccY - prevAccY) < 0.1){
    tendencyY = 0;
  }

  if((realAccY - prevAccY > 0 && tendencyY == 1) || (realAccY- prevAccY < 0 && tendencyY ==-1) ){
      accYArr[i] = realAccY;


      velY = ((realAccY) * 0.2);

      posY += (velY)*0.2;
      Serial.println(posY);
     
  }else{
    Serial.println(posY);
  }

  posYArr[i] = posY;

  delay(200);
  i++;
  }

  for(auto i: posYArr){
      Serial.print(i);
      Serial.print(",");
  }
  delay(500000);
} 

