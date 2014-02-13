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

