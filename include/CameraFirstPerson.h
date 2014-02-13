/*
 * Kamera ibiltaria
 * Aurrera/atzera/eskuin/ezkerrara mugi daiteke.
 * X eta Y ardatzetan bira daiteke (X ardatzean 90 gradutara mugatuta).
 */

#ifndef CAMERAFIRSTPERSON_H
#define	CAMERAFIRSTPERSON_H

#include "Definitions.h"
#include "ICamera.h"

class CameraFirstPerson : public ICamera {
public:
    CameraFirstPerson();

    // Move the position of the camera
    bool move(double x, double y, double z);
    // Rotate the view
    bool rotate(Axis axis, int angle);
    
    // Back to the starting position
    void reset();
    
private:
    // Current rotation angle on the X axis
    int rotationX_angle;
};

#endif	/* CAMERAFIRSTPERSON_H */

