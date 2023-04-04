
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
  guessOffsets();

  if(devStat == 0){
    nsMPU::mpu.setDMPEnabled(true);
  }

}
void MyMPU::getRollPitchYawAcceleration(double& realAccX,double& realAccY,double& realAccZ,double& gravityX, double& gravityY, double& gravityZ){
  uint8_t FiBuffer[64];
  Quaternion quat;

  VectorInt16 aa;
  VectorFloat v;
  if(nsMPU::mpu.dmpGetCurrentFIFOPacket(FiBuffer)){
      nsMPU::mpu.dmpGetQuaternion(&quat, FiBuffer);
      v.x = quat.x * quat.z - quat.w * quat.y;
      v.y = quat.w * quat.x + quat.y * quat.z;
      v.z = quat.w * quat.w - quat.x * quat.x - quat.y * quat.y + quat.z * quat.z;

      nsMPU::mpu.dmpGetAccel(&aa, FiBuffer);
      nsMPU::mpu.dmpGetGravity(&v, &quat);

      gravityX = v.x*9.81f;
      gravityY = v.y*9.81f;
      gravityZ = v.z*9.81f;

      realAccX = aa.x*9.81f; 
      realAccY = aa.y*9.81f; 
      realAccZ = aa.z*9.81f;


      
  } 


}


void MyMPU::guessOffsets(){
    nsMPU::mpu.setXGyroOffset(220);
    nsMPU::mpu.setYGyroOffset(76);
    nsMPU::mpu.setZGyroOffset(-85);
    nsMPU::mpu.setZAccelOffset(1788);

}