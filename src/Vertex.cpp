#include <sstream>
#include "Vertex.h"

Vertex::Vertex(std::string line) :
    face_table(0), normal(0, 0, 0), coord(0, 0, 0)
{
    // Erpinaren puntuak irakurri
    char v;
    std::istringstream iss(line);
    iss >> v; // v letra irakurri
    iss >> coord.v[0] >> coord.v[1] >> coord.v[2]; // Puntuak irakurri
}
