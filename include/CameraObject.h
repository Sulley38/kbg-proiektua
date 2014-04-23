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
 * Objektu-kamera
 * Beste edozein objektu bezala aritzen du.
 * Edozein ardatzetan mugi eta bira daiteke.
 */

#ifndef CAMERAOBJECT_H
#define	CAMERAOBJECT_H

#include "Definitions.h"
#include "ICamera.h"
#include "Object3D.h"

class CameraObject : public Object3D, public ICamera {
public:
    CameraObject();
    
    // Redefine object transformations in order to change the camera position properly
    void move(double x, double y, double z, bool objectReference);
    void scale(double fx, double fy, double fz, bool objectReference);
    void rotate(Axis axis, int angle, bool objectReference);
    
    // Redefine transformation undo/redo in order to change the camera position properly
    void undo();
    void redo();
    
    // Move the position of the camera
    bool move(double x, double y, double z);
    // Rotate the view
    bool rotate(Axis axis, int angle);
    
    // Back to the starting position
    void reset();
    
protected:
    // Multiply the transformation matrix with the camera position (eye, at, up)
    void updatePosition();
};

#endif	/* CAMERAOBJECT_H */

