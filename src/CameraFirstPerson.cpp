/*  Copyright 2013 Iván Matellanes, Adrián Núñez
 *
 *  This file is part of KbG Proiektua.
 *
 *  KbG Proiektua is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or (at your
 *  option) any later version.
 *
 *  KbG Proiektua is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 *  Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with KbG Proiektua is distributed. 
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cmath>
#include "CameraFirstPerson.h"

CameraFirstPerson::CameraFirstPerson() : rotationX_angle(0) {}

bool CameraFirstPerson::move(double x, double y, double z) {
    if (y != 0.0) {
        // Camera can not move up or down, Y axis is not available
        return false;
    } else {
        // Translate eye and lookAt points
        eye.v[0] += x;
        eye.v[2] += z;
        at.v[0] += x;
        at.v[2] += z;
        return true;
    }
}

bool CameraFirstPerson::rotate(Axis axis, int angle) {
    double radAngle;
    double SIN, COS;
    Array3 newAt(at), newUp(up);
    // Transform lookAt point and up vector according to the angle
    switch (axis) {
    case X:
        if ((rotationX_angle != -90 || angle > 0) && (rotationX_angle != 90 || angle < 0)) {
            rotationX_angle += angle;
            // Hold the rotation angle inside the limits
            if (rotationX_angle < -90) {
                rotationX_angle = -90;
                radAngle = ((-90 - rotationX_angle) * M_PI) / 180.0;
            } else if (rotationX_angle > 90) {
                rotationX_angle = 90;
                radAngle = ((90 - rotationX_angle) * M_PI) / 180.0;
            } else {
                radAngle = (angle * M_PI) / 180.0;
            }
            SIN = sin(radAngle), COS = cos(radAngle);
            // Update camera position
            newAt.v[1] = COS * at.v[1] - SIN * at.v[2] + eye.v[1] - COS * eye.v[1] + SIN * eye.v[2];
            newAt.v[2] = SIN * at.v[1] + COS * at.v[2] - SIN * eye.v[1] + eye.v[2] - COS * eye.v[2];
            newUp.v[1] = COS * up.v[1] - SIN * up.v[2];
            newUp.v[2] = SIN * up.v[1] + COS * up.v[2];
            at = newAt, up = newUp;
        }
        return true;
    case Y:
        radAngle = (angle * M_PI) / 180.0;
        SIN = sin(radAngle), COS = cos(radAngle);
        // Update camera position
        newAt.v[0] = COS * at.v[0] + SIN * at.v[2] + eye.v[0] - COS * eye.v[0] - SIN * eye.v[2];
        newAt.v[2] = -SIN * at.v[0] + COS * at.v[2] + SIN * eye.v[0] + eye.v[2] - COS * eye.v[2];
        newUp.v[0] = COS * up.v[0] + SIN * up.v[2];
        newUp.v[2] = -SIN * up.v[0] + COS * up.v[2];
        at = newAt, up = newUp;
        return true;
    case Z:
        return false;
    }
    return false;
}

void CameraFirstPerson::reset() {
    // Back to the original position
    eye = Array3(0, 5, 5);
    at = Array3(0, 5, 0);
    up = Array3(0, 1, 0);
    fovy = 60;
    rotationX_angle = 0;
}
