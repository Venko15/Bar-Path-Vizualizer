#pragma once
#include "InterfaceMPU.h"
class MyMPU: public InterfaceMPU{
  virtual void Init() override;
  virtual void getRealAcceleration(double& realAccX,double& realAccY,double& realAccZ,double& gravityX, double& gravityY, double& gravityZ) override;

};