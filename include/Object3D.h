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
 * 3D objektuaren egitura
 */

#ifndef OBJECT3D_H
#define	OBJECT3D_H

#include <QListWidgetItem>
#include <QString>
#include <list>
#include <vector>
#include "Definitions.h"
#include "Face.h"
#include "Material.h"
#include "Vertex.h"

class Object3D : public QListWidgetItem {
public:
    // Constructor from .obj file, indicate if it's a camera or not
    Object3D(const QString& path, const int id = -1, const bool isCamera = false);
    ~Object3D();
    
    // Get identifier
    int getId() const { return id; }
    // Get the position of the object
    const Array4& getPosition() const { return position; }
    Array3 getPosition3() const { return Array3(position.v[0], position.v[1], position.v[2]); }
    // Get dimensions
    const Array3& getMin() const { return min; }
    const Array3& getMax() const { return max; }
    // Set the position of the object
    void setPosition(const Array4& p) { position = p; }
    void setPosition3(const Array3& p) { position.v[0] = p.v[0], position.v[1] = p.v[1], position.v[2] = p.v[2]; }
    
    // Display the object on screen using its material or the color if the material can't be used
    // Also draws a bounding box if lightsEnabled = true and boundingBox = true
    virtual void draw(const bool lightsEnabled, const QColor& color, const bool boundingBox = false);
    
    // Load a material from file and assign it to the object
    bool hasMaterial() const { return (material != NULL); }
    void loadMaterial(const char* filename) { material = new Material(filename); }
    void clearMaterial() { delete material; material = NULL; }

    // Get and set the related object or camera
    int getRelation() const { return relation; }
    void setRelation(const int rel) { relation = rel; }
    
    // Apply transformations
    virtual void move(double x, double y, double z, bool objectReference);
    virtual void scale(double fx, double fy, double fz, bool objectReference);
    virtual void rotate(Axis axis, int angle, bool objectReference);
    
    // Undo last transformation and check if there are operations to undo
    virtual void undo();
    bool undoable() const { return !undo_buffer.empty(); }
    
    // Redo undone tranformations and check if there are operations to redo
    virtual void redo();
    bool redoable() const { return !redo_buffer.empty(); }
    
    // Reset all transformations done to the object, returning to its original state
    virtual void reset();
    
protected:       
    // Read an .obj file and fill this object with its representation data
    int load(const char *filename);
    // Center the object's coordinates on the (X = 0, Z = 0) and min.Y = 0
    void centerOnOrigin();
    // Compute normal vectors for each face and vertex of the object
    void calculateNormalVectors();
    
    // Multipy the current transformation matrix with the given one
    // If postmultiply = true, post-multiplication is done, pre-multiplication otherwise
    void pushMatrix(const std::vector<double>& transf_matrix, bool postmultiply);
    // Get the current transformation matrix
    const std::vector<double>& tranformationMatrix() const { return matrix; }
    
private:
    static int objectCount; // Count for object identifiers
    
    int id; // Unique identifier of the object
    std::vector<Vertex> vertex_table; // Table of vertices
    std::vector<Face> face_table; // Table of faces
    
    Array4 position; // Position of the center of the object
    Array3 min, max; // Upper and lower bounds of coordinates
    
    // Current transformation matrix
    std::vector<double> matrix;
    // List of old transformation matrices for undoing
    std::list< std::vector<double> > undo_buffer;
    // List of undone transformations available for redoing
    std::list< std::vector<double> > redo_buffer;
    
    // Object material
    Material *material;
    
    // Identifier of the relationship with a light
    int relation;
};

#endif	/* OBJECT3D_H */
