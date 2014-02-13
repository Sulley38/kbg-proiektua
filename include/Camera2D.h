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

