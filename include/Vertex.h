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