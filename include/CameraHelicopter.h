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

/*
 * Helikoptero-kamera
 * Norabide guztietan mugi eta Y ardatzean bira daiteke.
 */

#ifndef CAMERAHELICOPTER_H
#define	CAMERAHELICOPTER_H

#include "Definitions.h"
#include "ICamera.h"

class CameraHelicopter : public ICamera {
public:
    CameraHelicopter();
    
    // Move the position of the camera
    bool move(double x, double y, double z);
    // Rotate the view
    bool rotate(Axis axis, int angle);
    
    // Back to the starting position
    void reset();
};

#endif	/* CAMERAHELICOPTER_H */

