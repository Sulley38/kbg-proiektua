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

#ifndef DEFINITIONS_H
#define	DEFINITIONS_H

#include <cstdio>
#include <QColor>

#ifdef __DEBUG__
#define TRACE( ... ) fprintf(stdout, __VA_ARGS__);
#else
#define TRACE( ... ) ;
#endif
#define ERROR( ... ) fprintf(stderr, __VA_ARGS__);

#define WIDTH 1000
#define HEIGHT 600

#define MAX_LIGHTS 8

enum Target { TObject, TCamera, TLight };
enum Operation { Move, Scale, Rotate };
enum Axis { X, Y, Z };
enum CameraType { Ortho, Helicopter, ObjectCam, FirstPerson };
enum LightType { Sun, Lightbulb, Spotlight };

struct Array3 {
    Array3() { Array3(0, 0, 0); }
    Array3(const Array3& t) { v[0] = t.v[0], v[1] = t.v[1], v[2] = t.v[2]; }
    Array3(float a, float b, float c) { v[0] = a, v[1] = b, v[2] = c; }
    float v[3];
};

struct Array4 {
    Array4() { Array4(0, 0, 0, 0); }
    Array4(const Array4& t) { v[0] = t.v[0], v[1] = t.v[1], v[2] = t.v[2], v[3] = t.v[3]; }
    Array4(float a, float b, float c, float d) { v[0] = a, v[1] = b, v[2] = c, v[3] = d; }
    float v[4];
};

struct Settings {
    bool quadView, lightsEnabled, showGrid, showSightLine;
    int gridSize, gridStep;
    QColor bgColor, selectedColor, cameraColor, objectColor, gridColor;
};

#endif	/* DEFINITIONS_H */
