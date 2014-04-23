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
 * Erpinen egitura
 */

#ifndef VERTEX_H
#define	VERTEX_H

#include <vector>
#include <string>
#include "Definitions.h"

class Vertex {
public:
    // Creates a vertex by parsing a description line
    Vertex(std::string line);
    
    // Add a face index to the table
    void addFace(const int index) { face_table.push_back(index); }
    // Get number of faces in the table
    int getFaceCount() const { return face_table.size(); }
    // Get a face by its index in the table
    int getFaceIndex(const int i) const { return face_table[i]; }
    // Get or set the normal vector of this vertex
    const Array3& getNormal() const { return normal; }
    void setNormal(const Array3& vn) { normal = vn; }
    // Get or set the coordinates of this vertex
    const Array3& getCoordinates() const { return coord; }
    void setCoordinates(const Array3& v) { coord = v; }
    
private:
    std::vector<int> face_table; // Indexes of the faces that share this vertex
    Array3 normal; // The average of the normal vectors of the faces that this vertex belongs to
    Array3 coord; // Coordinates X, Y, Z
};

#endif	/* VERTEX_H */
