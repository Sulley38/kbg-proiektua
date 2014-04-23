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
 * OpenGL argiaren errepresentazioa.
 * Hiru mota ezberdin daude: eguzkia, bonbilla eta fokua.
 */

#ifndef LIGHT_H
#define	LIGHT_H

#include "Definitions.h"
#include "Object3D.h"

class Light : public Object3D {
public:
    // Construct a light given its type
    Light(const LightType T, const int lightNumber);
    // Parse a light description file
    Light(const char* filename, const int lightNumber);
    
    // Redefine drawing in order to update the position of the light
    void draw(const bool lightsEnabled, const QColor& color, const bool boundingBox = false);
        
    // Redefine reset to adjust the spot direction
    void reset();
    
    // Get the type of light and if it is switched on
    LightType getType() const { return type; }
    bool isOn() const { return on; }
    
    // Set the position and direction of the light according to the camera values passed
    void followCamera(const Array3& eye, const Array3& at);
    
    // Toggle light on/off and return its actual status
    bool toggle();
    
    // Change the light type
    void changeType();
    
protected:
    // Load the parameters of the light into OpenGL system
    void setParameters();
    
private:
    // Light type
    LightType type;
    // Toggle light on/off
    bool on;
    
    // Intensities of the light
    Array4 ambient, diffuse, specular;
    // Direction, exponent and cutoff angle of the spotlight
    Array3 spotDirection;
    float spotExponent, spotCutoff;
    // Attenuation factors
    float constantAtt, linearAtt, quadraticAtt;
};

#endif	/* LIGHT_H */
