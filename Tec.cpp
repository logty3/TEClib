#include "Tec.h"

double TECmapSpher(std::vector<IFileData>& iFileData, DateTime time, std::array<double, 2> point){
    double lon = point[0] * 180 / M_PI;
    double lat = point[1] * 180 / M_PI;

    if (lat > 87.5 || lat < -87.5) return 9999;

    double iSec, timeSec = time.getUTCTime();
    std::size_t i,j,k;

    for ( i = 0; i < iFileData.size(); i++ ) {
        iSec = iFileData[i].moment.getUTCTime();
        if (timeSec < iSec) break;
    }

    double iprevSec = iFileData[i-1].moment.getUTCTime();

    for ( j = 0; j < 71; j++ ) {
        if (lat > iFileData[i].lat[j]) break;
    }

    for ( k = 0; k < 73; k++ ) {
        if (lon < iFileData[i].lon[k]) break;
    }


    if (iFileData[i - 1].values[j - 1][k - 1] == 9999 ||
        iFileData[i - 1].values[j - 1][k    ] == 9999 ||
        iFileData[i - 1].values[j    ][k - 1] == 9999 ||
        iFileData[i - 1].values[j    ][k    ] == 9999 ||
        iFileData[i    ].values[j - 1][k - 1] == 9999 ||
        iFileData[i    ].values[j - 1][k    ] == 9999 ||
        iFileData[i    ].values[j    ][k - 1] == 9999 ||
        iFileData[i    ].values[j    ][k    ] == 9999) {
        return 9999;
    }

    double tec = (
        iFileData[i - 1].values[j - 1][k - 1] * (iSec - timeSec) * (iFileData[i].lat[j] - lat) * (iFileData[i].lon[k] - lon) +
        iFileData[i - 1].values[j - 1][k] * (iSec - timeSec) * (iFileData[i].lat[j] - lat) * (lon - iFileData[i].lon[k - 1]) +
        iFileData[i - 1].values[j][k - 1] * (iSec - timeSec) * (lat - iFileData[i].lat[j - 1]) * (iFileData[i].lon[k] - lon) +
        iFileData[i - 1].values[j][k] * (iSec - timeSec) * (lat - iFileData[i].lat[j - 1]) * (lon - iFileData[i].lon[k - 1]) +

        iFileData[i].values[j - 1][k - 1] * (timeSec - iprevSec) * (iFileData[i].lat[j] - lat) * (iFileData[i].lon[k] - lon) +
        iFileData[i].values[j - 1][k] * (timeSec - iprevSec) * (iFileData[i].lat[j] - lat) * (lon - iFileData[i].lon[k - 1]) +
        iFileData[i].values[j][k - 1] * (timeSec - iprevSec) * (lat - iFileData[i].lat[j - 1]) * (iFileData[i].lon[k] - lon) +
        iFileData[i].values[j][k] * (timeSec - iprevSec) * (lat - iFileData[i].lat[j - 1]) * (lon - iFileData[i].lon[k - 1])) /
        ((iSec - iprevSec) * (iFileData[i].lat[j] - iFileData[i].lat[j - 1]) * (iFileData[i].lon[k] - iFileData[i].lon[k - 1]));

    return tec/10;
};

double TECmapDec(std::vector<IFileData>& iFileData, DateTime time, std::array<double, 3> point){
    return TECmapSpher(iFileData, time, DecToSpher(point));
};

double IPPTEC(std::vector<IFileData>& iFileData, DateTime time, std::array<double, 3>& satPos, std::array<double, 3>& pointPos){
    double A = std::pow(satPos[0] - pointPos[0], 2) + std::pow(satPos[1] - pointPos[1], 2) + std::pow(satPos[2] - pointPos[2], 2);
    double B = 2 * ((satPos[0] - pointPos[0]) * pointPos[0] + (satPos[1] - pointPos[1]) * pointPos[1] + (satPos[2] - pointPos[2]) * pointPos[2]);
    double C = std::pow(pointPos[0], 2) + std::pow(pointPos[1], 2) + std::pow(pointPos[2], 2) - std::pow(RC + H_ION, 2);
    double t[2];
    t[0] = (-B + std::sqrt(B*B - 4 * A*C)) / (2 * A);
    t[1] = (-B - std::sqrt(B*B - 4 * A*C)) / (2 * A);

    std::array<double, 3> point1, point2;

    point1[0] = (satPos[0] - pointPos[0])*t[0] + pointPos[0];
    point1[1] = (satPos[1] - pointPos[1])*t[0] + pointPos[1];
    point1[2] = (satPos[2] - pointPos[2])*t[0] + pointPos[2];

    point2[0] = (satPos[0] - pointPos[0])*t[1] + pointPos[0];
    point2[1] = (satPos[1] - pointPos[1])*t[1] + pointPos[1];
    point2[2] = (satPos[2] - pointPos[2])*t[1] + pointPos[2];

    if (std::sqrt(std::pow(point1[0] - pointPos[0], 2) + std::pow(point1[1] - pointPos[1], 2) + std::pow(point1[2] - pointPos[2], 2)) < std::sqrt(std::pow(point2[0] - pointPos[0], 2) + std::pow(point2[1] - pointPos[1], 2) + std::pow(point2[2] - pointPos[2], 2))) {
        return TECmapDec(iFileData, time, point1);
    }

    return TECmapDec(iFileData, time, point2);
}
