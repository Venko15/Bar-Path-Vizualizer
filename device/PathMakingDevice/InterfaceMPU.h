#pragma once

class InterfaceMPU{
  public:
    virtual void Init() = 0;
    virtual void getRealAcceleration(double& realAccX,double& realAccY,double& realAccZ,double& gravityX, double& gravityY, double& gravityZ) = 0;
};