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
 * Aurpegien egitura
 */

#ifndef FACE_H
#define	FACE_H

#include <vector>
#include <string>
#include "Definitions.h"

class Face {
public:
    // Create a face by parsing a description line
    Face(std::string line);
    
    // Add a vertex that belongs to the face
    void addVertex(const int index) { vertex_table.push_back(index); }
    // Get number of vertices in the table
    int getVerticesCount() const { return vertex_table.size(); }
    // Get a vertex by its index in the table
    int getVertexIndex(const int i) const { return vertex_table[i]; }
    // Get or set the normal vector
    const Array3& getNormal() const { return normal; }
    void setNormal(const Array3& vn) { normal = vn; }
    
private:
    std::vector<int> vertex_table; // Table with the index of each vertex
    Array3 normal; // Normal vector of the face
};

#endif	/* FACE_H */

