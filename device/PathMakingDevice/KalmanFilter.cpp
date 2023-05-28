#include "KalmanFilter.h"

KalmanFilter::KalmanFilter(double initial_x, double initial_P, double process_noise, double measurement_noise)
    : x(initial_x), P(initial_P), Q(process_noise), R(measurement_noise) {}

double KalmanFilter::update(double measurement) {
    // Prediction step
    double x_pred = x;
    double P_pred = P + Q;

    // Measurement update step
    double K = P_pred / (P_pred + R);
    x = x_pred + K * (measurement - x_pred);
    P = (1 - K) * P_pred;

    return x;
}

void KalmanFilter::reset(double initial_x, double initial_P, double process_noise, double measurement_noise){
  x=initial_x;
  P=initial_P;
  Q=process_noise;
  R=measurement_noise;
}

