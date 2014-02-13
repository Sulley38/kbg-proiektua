#include <cmath>
#include "CameraHelicopter.h"

CameraHelicopter::CameraHelicopter() {}

bool CameraHelicopter::move(double x, double y, double z) {
    // Translate eye and lookAt points
    eye.v[0] += x;
    eye.v[1] += y;
    eye.v[2] += z;
    at.v[0] += x;
    at.v[1] += y;
    at.v[2] += z;
    return true;
}

bool CameraHelicopter::rotate(Axis axis, int angle) {
    // Transform lookAt point and up vector according to the angle
    if (axis == Y) {
        double radAngle = (angle * M_PI) / 180.0;
        double SIN = sin(radAngle), COS = cos(radAngle);
        Array3 newAt(at), newUp(up);
        newAt.v[0] = COS * at.v[0] + SIN * at.v[2] + eye.v[0] - COS * eye.v[0] - SIN * eye.v[2];
        newAt.v[2] = -SIN * at.v[0] + COS * at.v[2] + SIN * eye.v[0] + eye.v[2] - COS * eye.v[2];
        newUp.v[0] = COS * up.v[0] + SIN * up.v[2];
        newUp.v[2] = -SIN * up.v[0] + COS * up.v[2];
        at = newAt, up = newUp;
        return true;
    }
    return false;
}

void CameraHelicopter::reset() {
    // Back to the original position
    eye = Array3(0, 5, 5);
    at = Array3(0, 5, 0);
    up = Array3(0, 1, 0);
    fovy = 60;
}