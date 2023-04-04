#include "InterfaceMPU.h"
#include "Configuire.h"
#include "HighPassFilter.h"

double  accX, accY, accZ;
Config con;
InterfaceMPU &mpu = con.getMPU();

HighPassFilter filter;

void setup() {
  HighPassFilter_init(&filter);
  Serial.begin(115200);
  while(!Serial);
  mpu.Init();

}
void loop() {
  double y, p, r;
  mpu.getRollPitchYawAcceleration(r,p,y, accX, accY, accZ);
  HighPassFilter_putX(&filter,accX-r/(8192));
  HighPassFilter_putY(&filter,accY-p/(8192));
  HighPassFilter_putZ(&filter,accZ-y/(8192));
  Serial.print(HighPassFilter_getX(&filter));
  Serial.print("   ");
  Serial.print(HighPassFilter_getY(&filter));
  Serial.print("   ");
  Serial.println(HighPassFilter_getZ(&filter));

  delay(200);


} 
