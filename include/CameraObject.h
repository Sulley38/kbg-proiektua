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

