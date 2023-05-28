
#include "MyMPU.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUIN_WIRE
  #include "Wire.h"
#endif

namespace nsMPU{
  MPU6050 mpu;
};

void MyMPU::Init(){
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        Wire.setClock(400000); 
    #endif

  nsMPU::mpu.initialize();
  nsMPU::mpu.testConnection();
  uint8_t devStat = nsMPU::mpu.dmpInitialize();

  if(devStat == 0){
    nsMPU::mpu.setDMPEnabled(true);
  }

}

void MyMPU::getRealAcceleration(double& realAccX,double& realAccY,double& realAccZ,double& gravityX, double& gravityY, double& gravityZ){
  uint8_t FiBuffer[64];
  Quaternion quat;
  VectorInt16 aa;
  VectorFloat v;

  if(nsMPU::mpu.dmpGetCurrentFIFOPacket(FiBuffer)){
    
      nsMPU::mpu.dmpGetQuaternion(&quat, FiBuffer);
      nsMPU::mpu.dmpGetAccel(&aa, FiBuffer);
      nsMPU::mpu.dmpGetGravity(&v, &quat);
      
      realAccX = (aa.x*9.81f/8192 - v.x*9.81f);
      realAccY = (aa.y*9.81f/8192 - v.y*9.81f);
      realAccZ = (aa.z*9.81f/8192 -v.z*9.81f);


      
  } 
}

