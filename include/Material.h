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