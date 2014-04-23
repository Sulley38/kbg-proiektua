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
