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

