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

#include <GL/gl.h>
#include "Camera2D.h"

Camera2D::Camera2D() :
    left(-10.0), right(10.0), bottom(-10.0), top(10.0), near(-1000.0), far(1000.0),
    zoomFactor(1.0) {}

bool Camera2D::move(double x, double y, double z) {
    // Move the projection clipping bounds in the corresponding direction
    left += x;
    right += x;
    bottom += y - z;
    top += y - z;
    return true;
}

bool Camera2D::rotate(Axis, int) {
    // This camera cannot be rotated
    return false;
}

void Camera2D::zoom(double factor) {
    // Save the zoom setting
    zoomFactor = -factor + 2;
}

void Camera2D::project(double aspectRatio) {
    // Calculate projection limits with zooming applied
    double offset = (abs(right - left) * zoomFactor - abs(right - left)) / 2.0;
    double zLeft = left - offset, zRight = right + offset,
           zBottom = bottom - offset, zTop = top + offset;
    // Set ortographic projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (aspectRatio >= (zRight - zLeft) / (zTop - zBottom)) {
        // Ezker eta eskuineko marjinak = Eszenaren zabalerari falta zaiona leihoaren zabalera/altuera proportzioa berdintzeko
        offset = (((zTop - zBottom) * aspectRatio) - (zRight - zLeft)) / 2.0;
        glOrtho(zLeft - offset, zRight + offset, zBottom, zTop, near, far);
    } else {
        // Goi eta beheko marjinak = Eszenaren altuerari falta zaiona leihoaren zabalera/altuera proportzioa berdintzeko
        offset = (((zRight - zLeft) / aspectRatio) - (zTop - zBottom)) / 2.0;
        glOrtho(zLeft, zRight, zBottom - offset, zTop + offset, near, far);
    }
    glMatrixMode(GL_MODELVIEW);
}

void Camera2D::reset() {
    // Reset the projection volume
    left = bottom = -10.0;
    right = top = 10.0;
    zoomFactor = 1.0;
}
