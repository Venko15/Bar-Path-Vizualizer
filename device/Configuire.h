#pragma once 
#include "InterfaceMPU.h"
#include "MyMPU.h"

class Config{
  private:
    MyMPU mpu;
  public:
    InterfaceMPU& getMPU();

};