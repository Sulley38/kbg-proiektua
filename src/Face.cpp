#include <sstream>
#include "Face.h"

Face::Face(std::string line) :
    vertex_table(), normal(0, 0, 0)
{
    // Aurpegiaren erpinak irakurri
    int v;
    std::string junk;
    std::istringstream iss(line);
    iss >> junk; // f letra irakurri
    while (iss >> v) { // Erpina irakurri
        if (iss.peek() == '/')
                iss >> junk; // Hurrengo hutsunerarte aurreratu
        addVertex(v - 1);
    }
}
