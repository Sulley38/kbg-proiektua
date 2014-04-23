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
 * Bi dimentsioetako kamera (proiektzio ortogonala)
 */

#ifndef CAMERA2D_H
#define	CAMERA2D_H

#include <vector>
#include "ICamera.h"
#include "Object3D.h"

class Camera2D : public ICamera {
public:
    Camera2D();
    
    // Move the projection bounds up/down/left/right
    bool move(double x, double y, double z);
    // Rotate the camera location
    bool rotate(Axis axis, int angle);
    // Increase or decrease the projection size (i.e., zoom in or out)
    void zoom(double factor);
    
    // Set the proper projection matrix for this camera (glOrtho)
    void project(double aspectRatio);
   
    // Reset the view to the origin
    void reset();
    
private:
    // Ortographic projection clipping bounds
    double left, right, bottom, top, near, far;
    
    // Zooming factor
    double zoomFactor;
};

#endif	/* CAMERA2D_H */

