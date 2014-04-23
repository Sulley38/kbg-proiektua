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
