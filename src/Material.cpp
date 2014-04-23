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

#include <fstream>
#include "Definitions.h"
#include "Material.h"

Material::Material() : ambient(0.19125, 0.0735, 0.0225, 1.0),
    diffuse(0.7038, 0.27048, 0.0828, 1.0),
    specular(0.256777, 0.137622, 0.0806014, 1.0),
    emission(0.0, 0.0, 0.0, 1.0),
    shininess(0.1) {}

Material::Material(const char* filename) {
    // Parse the material description file
    std::ifstream file(filename);
    file >> ambient.v[0] >> ambient.v[1] >> ambient.v[2];
    ambient.v[3] = 1.0;
    file >> diffuse.v[0] >> diffuse.v[1] >> diffuse.v[2];
    diffuse.v[3] = 1.0;
    file >> specular.v[0] >> specular.v[1] >> specular.v[2];
    specular.v[3] = 1.0;
    file >> emission.v[0] >> emission.v[1] >> emission.v[2];
    emission.v[3] = 1.0;
    file >> shininess;
    file.close();
}
