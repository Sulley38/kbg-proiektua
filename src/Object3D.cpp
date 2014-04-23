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

#include <cmath>
#include <fstream>
#include <GL/gl.h>
#include <QDir>
#include "Object3D.h"

// Initialize counters
int Object3D::objectCount = 0;

Object3D::Object3D(const QString& path, const int id, const bool isCamera) :
    QListWidgetItem(0, 1001),
    // Initialize variables and containers
    id(id),
    vertex_table(), face_table(),
    position(0, 0, 0, 1), min(0, 0, 0), max(0, 0, 0),
    matrix(16, 0.0), undo_buffer(), redo_buffer(),
    material(new Material()), relation(-1)
{
    // Assign an unique sequential identifier (if one not given)
    if (id == -1) this->id = objectCount++;
    
    // Push identity matrix
    matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0;
    
    // Load object file
    if (load(path.toUtf8().constData()) == 0) {
        // Move the object so that it "touches the ground" (min.v[1] = 0)
        move(0.0, -min.v[1], 0.0, true);
        if (isCamera)
            setText("Kamera-objektua");
        else
            setText(QString("%1: " + path.mid(path.lastIndexOf(QDir::separator()) + 1)).arg(this->id));
    }
}

Object3D::~Object3D() {
    if (material != 0)
        delete material;
}

/* Function to read obj files and load them in an Object3D class.
   List of error codes:
   -1 -> File not found or empty
   -2 -> There are no vertices and/or faces */
int Object3D::load(const char *filename) {
    std::string line;
    int num_vertices = 0, num_faces = 0, i, j;
    
    /* We parse the file adding vertices and faces to the object */
    std::ifstream file(filename);
    if (!file.good()) {
        ERROR("File not found: (%s)\n", filename);
        return -1;
    }
    
    /* Remove previous content (if any) */
    vertex_table.clear();
    face_table.clear();
    
    /* Parse the lines */
    while (!file.eof()) {
        getline(file, line);
        if ((i = line.find_first_not_of(' ')) != int(std::string::npos)) {
            if (line[i] == 'f' && line[i + 1] == ' ') {
                // Add a face to the object
                face_table.push_back(Face(line));
                // For each vertex of the face, add the face to its face table
                for (j = 0; j < face_table[num_faces].getVerticesCount(); j++)
                    vertex_table[face_table[num_faces].getVertexIndex(j)].addFace(num_faces);
                num_faces++;
            } else if (line[i] == 'v' && line[i + 1] == ' ') {
                // Add a vertex to the object
                vertex_table.push_back(Vertex(line));
                num_vertices++;
            }
        }
    }
    file.close();

    /* Check if there are contents */
    if (num_vertices == 0) {
        ERROR("No vertex found: (%s)\n", filename);
        id = -1;
        return -2;
    }
    if (num_faces == 0) {
        ERROR("No face found: (%s)\n", filename);
        id = -1;
        return -2;
    }

    /* Now we look for the maximum and minimum coordinates */
    max = vertex_table[0].getCoordinates();
    min = vertex_table[0].getCoordinates();

    Array3 vertex;
    for (i = 1; i < num_vertices; i++) {
        vertex = vertex_table[i].getCoordinates();
        min.v[0] = std::min(min.v[0], vertex.v[0]);
        min.v[1] = std::min(min.v[1], vertex.v[1]);
        min.v[2] = std::min(min.v[2], vertex.v[2]);
        max.v[0] = std::max(max.v[0], vertex.v[0]);
        max.v[1] = std::max(max.v[1], vertex.v[1]);
        max.v[2] = std::max(max.v[2], vertex.v[2]);
    }
    
    /* Center the object and calculate its normal vectors */
    centerOnOrigin();
    calculateNormalVectors();
    
    return 0;
}

void Object3D::centerOnOrigin() {
    // Change coordinates so that object is placed on the origin
    double dx = -min.v[0] - ((max.v[0] - min.v[0]) / 2.0),
           dy = -min.v[1] - ((max.v[1] - min.v[1]) / 2.0),
           dz = -min.v[2] - ((max.v[2] - min.v[2]) / 2.0);
    // Update every vertex with the offset
    Array3 vertex;
    for (int i = 0; i < (int)vertex_table.size(); i++) {
        vertex = vertex_table[i].getCoordinates();
        vertex.v[0] += dx, vertex.v[1] += dy, vertex.v[2] += dz;
        vertex_table[i].setCoordinates(vertex);
    }
    // Update max and min points
    min.v[0] += dx, min.v[1] += dy, min.v[2] += dz;
    max.v[0] += dx, max.v[1] += dy, max.v[2] += dz;
}

void Object3D::calculateNormalVectors() {
    Array3 p1, p2, p3;
    Array3 v1, v2, vn;
    double norm;
    // Compute the normal of each face
    for (int i = 0; i < (int)face_table.size(); ++i) {
        // Fetch vertices
        p1 = vertex_table[face_table[i].getVertexIndex(0)].getCoordinates();
        p2 = vertex_table[face_table[i].getVertexIndex(1)].getCoordinates();
        p3 = vertex_table[face_table[i].getVertexIndex(2)].getCoordinates();
        // Calculate face vectors
        v1.v[0] = p2.v[0] - p1.v[0];
        v1.v[1] = p2.v[1] - p1.v[1];
        v1.v[2] = p2.v[2] - p1.v[2];
        v2.v[0] = p3.v[0] - p1.v[0];
        v2.v[1] = p3.v[1] - p1.v[1];
        v2.v[2] = p3.v[2] - p1.v[2];
        // Vector product of V1 and V2
        vn.v[0] = v1.v[1] * v2.v[2] - v1.v[2] * v2.v[1];
        vn.v[1] = v1.v[2] * v2.v[0] - v1.v[0] * v2.v[2];
        vn.v[2] = v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0];
        // Normalize VN vector
        norm = sqrt(vn.v[0] * vn.v[0] + vn.v[1] * vn.v[1] + vn.v[2] * vn.v[2]);
        vn.v[0] /= norm, vn.v[1] /= norm, vn.v[2] /= norm;
        face_table[i].setNormal(vn);
    }
    // Compute the normal of each vertex
    int faceCount;
    for (int i = 0; i < (int)vertex_table.size(); ++i) {
        vn.v[0] = 0.0, vn.v[1] = 0.0, vn.v[2] = 0.0;
        faceCount = vertex_table[i].getFaceCount();
        for (int j = 0; j < faceCount; ++j) {
            // For each face the vertex belongs to, add its normal
            vn.v[0] += face_table[vertex_table[i].getFaceIndex(j)].getNormal().v[0];
            vn.v[1] += face_table[vertex_table[i].getFaceIndex(j)].getNormal().v[1];
            vn.v[2] += face_table[vertex_table[i].getFaceIndex(j)].getNormal().v[2];
        }
        // Divide by the number of faces the vertex belongs to
        vn.v[0] /= faceCount, vn.v[1] /= faceCount, vn.v[2] /= faceCount;
        vertex_table[i].setNormal(vn);
    }
}

void Object3D::draw(const bool lightsEnabled, const QColor& color, const bool boundingBox) {
    // Load color or material properties
    if (material != 0 && lightsEnabled) {
        // Raw material
        glDisable(GL_COLOR_MATERIAL);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material->getAmbient());
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material->getDiffuse());
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material->getSpecular());
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material->getEmission());
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material->getShininess());
    } else {
        // Color-based material
        glEnable(GL_COLOR_MATERIAL);
        glColor3f(color.redF(), color.greenF(), color.blueF());
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Array4(0.0, 0.0, 0.0, 1.0).v);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Array4(0.0, 0.0, 0.0, 1.0).v);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
    }
    // Load transformations
    glMultMatrixd(&matrix[0]);
    // Draw vertices
    for (int i = 0; i < (int)face_table.size(); ++i) {
        // Polygons if lights enabled, line strips if not
        glBegin(lightsEnabled ? GL_POLYGON : GL_LINE_STRIP);
        for (int j = 0; j < face_table[i].getVerticesCount(); ++j) {
            glNormal3fv(vertex_table[face_table[i].getVertexIndex(j)].getNormal().v);
            glVertex3fv(vertex_table[face_table[i].getVertexIndex(j)].getCoordinates().v);
        }
        glEnd();
    }
    // Draw the bounding box
    if (lightsEnabled && boundingBox) {
        glEnable(GL_COLOR_MATERIAL);
        glColor3f(color.redF(), color.greenF(), color.blueF());
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Array4(0.0, 0.0, 0.0, 1.0).v);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Array4(0.0, 0.0, 0.0, 1.0).v);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
        glBegin(GL_LINE_LOOP);
            glVertex3f(min.v[0], min.v[1], min.v[2]);
            glVertex3f(min.v[0], min.v[1], max.v[2]);
            glVertex3f(min.v[0], max.v[1], max.v[2]);
            glVertex3f(min.v[0], max.v[1], min.v[2]);
        glEnd();
        glBegin(GL_LINE_LOOP);
            glVertex3f(max.v[0], min.v[1], min.v[2]);
            glVertex3f(max.v[0], min.v[1], max.v[2]);
            glVertex3f(max.v[0], max.v[1], max.v[2]);
            glVertex3f(max.v[0], max.v[1], min.v[2]);
        glEnd();
        glBegin(GL_LINES);
            glVertex3f(min.v[0], min.v[1], min.v[2]);
            glVertex3f(max.v[0], min.v[1], min.v[2]);
            glVertex3f(min.v[0], min.v[1], max.v[2]);
            glVertex3f(max.v[0], min.v[1], max.v[2]);
            glVertex3f(min.v[0], max.v[1], min.v[2]);
            glVertex3f(max.v[0], max.v[1], min.v[2]);
            glVertex3f(min.v[0], max.v[1], max.v[2]);
            glVertex3f(max.v[0], max.v[1], max.v[2]);
        glEnd();
    }
}

void Object3D::pushMatrix(const std::vector<double>& transf_matrix, bool postmultiply) {
    // Save current modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    // Push current transformation matrix to the undo buffer
    undo_buffer.push_front(matrix);
    // Perform multiplication through OpenGL and save the result
    if (postmultiply) {
        glLoadMatrixd(&matrix[0]);
        glMultMatrixd(&transf_matrix[0]);
    } else {
        glLoadMatrixd(&transf_matrix[0]);
        glMultMatrixd(&matrix[0]);
    }
    glGetDoublev(GL_MODELVIEW_MATRIX, &matrix[0]);
    // Update position of the object
    position.v[0] = matrix[12], position.v[1] = matrix[13], position.v[2] = matrix[14];
    // Clear redo buffer
    redo_buffer.clear();
    // Restore original modelview matrix
    glPopMatrix();
}

void Object3D::move(double x, double y, double z, bool objectReference) {
    // Build transformation matrix and push it to the stack
    std::vector<double> move_matrix(16, 0.0);
    move_matrix[0] = 1.0, move_matrix[5] = 1.0, move_matrix[10] = 1.0, move_matrix[15] = 1.0;
    move_matrix[12] = x, move_matrix[13] = y, move_matrix[14] = z;
    pushMatrix(move_matrix, objectReference);
}

void Object3D::scale(double fx, double fy, double fz, bool objectReference) {
    // Build transformation matrix and push it to the stack
    std::vector<double> scale_matrix(16, 0.0);
    scale_matrix[0] = fx, scale_matrix[5] = fy, scale_matrix[10] = fz, scale_matrix[15] = 1.0;
    pushMatrix(scale_matrix, objectReference);
}

void Object3D::rotate(Axis axis, int angle, bool objectReference) {
    // Build transformation matrix and push it to the stack
    std::vector<double> rotate_matrix(16, 0.0);
    double radAngle = (angle * M_PI) / 180.0;
    double SIN = sin(radAngle), COS = cos(radAngle);
    switch (axis) {
    case X:
        rotate_matrix[0] = 1.0;
        rotate_matrix[5] = COS, rotate_matrix[9] = SIN;
        rotate_matrix[6] = -SIN, rotate_matrix[10] = COS;
        rotate_matrix[15] = 1.0;
        break;
    case Y:
        rotate_matrix[0] = COS, rotate_matrix[8] = -SIN;
        rotate_matrix[5] = 1.0;
        rotate_matrix[2] = SIN, rotate_matrix[10] = COS;
        rotate_matrix[15] = 1.0;
        break;
    case Z:
        rotate_matrix[0] = COS, rotate_matrix[4] = SIN;
        rotate_matrix[1] = -SIN, rotate_matrix[5] = COS;
        rotate_matrix[10] = 1.0;
        rotate_matrix[15] = 1.0;
        break;
    }
    pushMatrix(rotate_matrix, objectReference);
}

void Object3D::undo() {
    if (undoable()) {
        redo_buffer.push_front(matrix); // Save undone tranformation for later redoing
        matrix = undo_buffer.front();
        undo_buffer.pop_front();
        position.v[0] = matrix[12], position.v[1] = matrix[13], position.v[2] = matrix[14]; // Update position of the object
    }
}

void Object3D::redo() {
    if (redoable()) {
        undo_buffer.push_front(matrix); // Save the transformation for undoing again
        matrix = redo_buffer.front();
        redo_buffer.pop_front();
        position.v[0] = matrix[12], position.v[1] = matrix[13], position.v[2] = matrix[14]; // Update position of the object
    }
}

void Object3D::reset() {
    // Clear undo/redo buffers
    undo_buffer.clear();
    redo_buffer.clear();
    // Set the identity matrix
    matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0;
    matrix[1] = matrix[2] = matrix[3] = matrix[4] = 0.0;
    matrix[6] = matrix[7] = matrix[8] = matrix[9] = 0.0;
    matrix[11] = matrix[12] = matrix[13] = matrix[14] = 0.0;
    // Move the object so that it "touches the ground" (min.v[1] = 0)
    move(0.0, -min.v[1], 0.0, true);
}
