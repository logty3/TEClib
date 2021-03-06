#include "utils.h"

std::array<double, 2> DecToSpher(std::array<double, 3>& point) {
    double e = (Ra * Ra - Rb * Rb) / (Ra * Ra);
    double e_ = (Ra * Ra - Rb * Rb) / (Rb * Rb);

    double tet = std::atan2(Ra * point[2], Rb * std::sqrt(point[0] * point[0] + point[1] * point[1]));

    return {
         std::atan2(point[1], point[0]),
         std::atan2(point[2] + e_ * e_ * Rb * std::pow(std::sin(tet), 3), std::sqrt(point[0] * point[0] + point[1] * point[1]) - e * e * Ra * std::pow(std::cos(tet), 3))
    };
}

std::vector<double> RK4(std::vector<double>(*der)(double, std::vector<double>), std::vector<double>& X1, double& t1, double& t2, const double& h1){
    std::vector<double> k1(X1.size()), k2(X1.size()), k3(X1.size()),k4(X1.size()), X(X1.size()), X2(X1.size());
    double t = t1;
    double h = h1;
    X2 = X1;
    while (t < t2) {
        X = X2;
        k1 = der(t, X2);

        std::size_t i = 0;
        for(auto &x: X){
            x = X2[i] + h * k1[i] / 2;
            i++;
        }

        k2 = der(t + h / 2, X);

        i = 0;
        for(auto &x: X){
            x = X2[i] + h * k2[i] / 2;
            i++;
        }

        k3 = der(t + h / 2, X);

        i = 0;
        for(auto &x: X){
            x = X2[i] + h * k3[i];
            i++;
        }

        k4 = der(t + h, X);

        i = 0;
        for(auto &x: X2){
            x = x +  h * (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]) / 6;
            i++;
        }
        t += h;
        if (t2 - t < h) h = t2 - t;
    }
    return X2;
}


