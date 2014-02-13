#include <GL/glu.h>
#include "ICamera.h"

ICamera::ICamera() : eye(0, 5, 5), at(0, 5, 0), up(0, 1, 0), fovy(60.0), relation(-1) {}

void ICamera::zoom(double factor) {
    // Sets the angle of the field of view
    // Factor values between 0.5 and 1.5 allowed
    fovy = 60 * (-factor + 2);
}

void ICamera::project(double aspectRatio) {
    // Perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, aspectRatio, 0.1, 500.0);
    glMatrixMode(GL_MODELVIEW);
}