#include "HighPassFilter.h"

static double filter_taps[HIGHPASSFILTER_TAP_NUM] = {
  0.054068430033061436,
  -0.012879687544701732,
  -0.32399138530043,
  0.5707233139958572,
  -0.32399138530043,
  -0.012879687544701732,
  0.054068430033061436
};

void HighPassFilter_init(HighPassFilter* f) {
  int i;
  for(i = 0; i < HIGHPASSFILTER_TAP_NUM; ++i){
    f->historyX[i] = 0;
    f->historyY[i] = 0;
    f->historyZ[i] = 0;
  }
  f->last_indexX = 0;
  f->last_indexY = 0;
  f->last_indexZ = 0;
}

void HighPassFilter_putX(HighPassFilter* f, double input) {
  f->historyX[f->last_indexX++] = input;
  if(f->last_indexX == HIGHPASSFILTER_TAP_NUM)
    f->last_indexX = 0;
}
void HighPassFilter_putY(HighPassFilter* f, double input) {
  f->historyY[f->last_indexY++] = input;
  if(f->last_indexY == HIGHPASSFILTER_TAP_NUM)
    f->last_indexY = 0;
}
void HighPassFilter_putZ(HighPassFilter* f, double input) {
  f->historyZ[f->last_indexZ++] = input;
  if(f->last_indexZ == HIGHPASSFILTER_TAP_NUM)
    f->last_indexZ = 0;
}

double HighPassFilter_getX(HighPassFilter* f) {
  double acc = 0;
  int index = f->last_indexX, i;
  for(i = 0; i < HIGHPASSFILTER_TAP_NUM; ++i) {
    index = index != 0 ? index-1 : HIGHPASSFILTER_TAP_NUM-1;
    acc += f->historyX[index] * filter_taps[i];
  };
  return acc;
}
double HighPassFilter_getY(HighPassFilter* f) {
  double acc = 0;
  int index = f->last_indexY, i;
  for(i = 0; i < HIGHPASSFILTER_TAP_NUM; ++i) {
    index = index != 0 ? index-1 : HIGHPASSFILTER_TAP_NUM-1;
    acc += f->historyY[index] * filter_taps[i];
  };
  return acc;
}
double HighPassFilter_getZ(HighPassFilter* f) {
  double acc = 0;
  int index = f->last_indexZ, i;
  for(i = 0; i < HIGHPASSFILTER_TAP_NUM; ++i) {
    index = index != 0 ? index-1 : HIGHPASSFILTER_TAP_NUM-1;
    acc += f->historyZ[index] * filter_taps[i];
  };
  return acc;
}
