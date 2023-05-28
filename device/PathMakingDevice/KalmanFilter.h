#ifndef KALMAN_FILTER_H
#define KALMAN_FILTER_H

class KalmanFilter {
private:
    double x;         // Estimated value
    double P;         // Estimated error covariance
    double Q;         // Process noise covariance
    double R;         // Measurement noise covariance

public:
    KalmanFilter(double initial_x, double initial_P, double process_noise, double measurement_noise);

    double update(double measurement);

    void reset(double initial_x, double initial_P, double process_noise, double measurement_noise);
};

#endif // KALMAN_FILTER_H
