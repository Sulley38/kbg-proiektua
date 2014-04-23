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
 * OpenGL Materialen errepresentazioa
 */

#ifndef MATERIAL_H
#define	MATERIAL_H

#include "Definitions.h"

class Material {
public:
    // Create a default material
    Material();
    // Create a material by parsing a description file
    Material(const char* filename);

    // Getter methods for the material properties
    const float* getAmbient() const { return ambient.v; }
    const float* getDiffuse() const { return diffuse.v; }
    const float* getSpecular() const { return specular.v; }
    const float* getEmission() const { return emission.v; }
    float getShininess() const { return shininess; }
    
private:
    Array4 ambient, diffuse, specular, emission; // Material color properties
    float shininess; // Material shininess
};

#endif	/* MATERIAL_H */
