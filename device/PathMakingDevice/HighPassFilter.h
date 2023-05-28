#ifndef HIGHPASSFILTER_H_
#define HIGHPASSFILTER_H_

/*

FIR filter designed with
 http://t-filter.appspot.com

sampling frequency: 5 Hz

* 0 Hz - 0.5 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = -45.81794621873907 dB

* 1.5 Hz - 2.5 Hz
  gain = 1
  desired ripple = 5 dB
  actual ripple = 2.1073045541086977 dB

*/

#define HIGHPASSFILTER_TAP_NUM 7

typedef struct {
  double historyX[HIGHPASSFILTER_TAP_NUM];
  double historyY[HIGHPASSFILTER_TAP_NUM];
  double historyZ[HIGHPASSFILTER_TAP_NUM];
  unsigned int last_indexX;
  unsigned int last_indexY;
  unsigned int last_indexZ;
} HighPassFilter;

void HighPassFilter_init(HighPassFilter* f);

void HighPassFilter_putX(HighPassFilter* f, double input);
void HighPassFilter_putY(HighPassFilter* f, double input);
void HighPassFilter_putZ(HighPassFilter* f, double input);
double HighPassFilter_getX(HighPassFilter* f);
double HighPassFilter_getY(HighPassFilter* f);
double HighPassFilter_getZ(HighPassFilter* f);

#endif