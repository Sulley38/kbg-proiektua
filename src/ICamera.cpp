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
