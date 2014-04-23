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
 * OpenGL Widget-a
 * Kamera mota ezberdinak eta hobespenak gordetzen ditu.
 * Eszena marrazteaz arduratzen da.
 */

#ifndef ENGINE_H
#define	ENGINE_H

#include <QGLWidget>
#include <QListWidget>
#include <vector>
#include "Definitions.h"
#include "ICamera.h"
#include "Light.h"
#include "Object3D.h"

class Engine : public QGLWidget {
    Q_OBJECT
    
public:
    Engine(QListWidget *objectList, QListWidget *lightList, QWidget *parent = 0);
    ~Engine();
    
    // Set a different camera
    ICamera* getCamera() { return camera[currentCamera]; }
    void setCamera(const CameraType c) { currentCamera = c; }
    
    // Relationship operations
    void createRelation(Light* light, Object3D* object = 0);
    Light* getRelatedLight(Object3D* object) { return objectRelations[object->getRelation()].second; }
    Light* getRelatedLight(ICamera* camera) { return cameraRelations[camera->getRelation()].second; }
    void removeRelation(const int rel);
    
    // Get or set the current program settings
    const Settings& getSettings() const { return settings; }
    void setSettings(const Settings& s);
    
public slots:
    void toggleQuadView() { settings.quadView = !settings.quadView; updateGL(); }
    void toggleLighting() { settings.lightsEnabled = !settings.lightsEnabled; updateGL(); }
    
signals:
    void zoom(int);

protected:
    // Set projections and views for each quadrant
    void topView(float width, float height);
    void frontView(float width, float height);
    void sideView(float width, float height);
    void userView(float width, float height);
    
    // Draw the grid, line of sight, lights and objects, and optionally the object camera
    void drawScene(const bool showObjectCamera = true);
    // Draw a grid on the background
    void drawGrid();
    // Draw the line of sight of the object camera
    void drawSightLine();
    
    // OpenGL events
    void initializeGL();
    void paintGL();
    // Mouse wheel event
    void wheelEvent(QWheelEvent *event);
    
private:
    // Pointer to the lists of objects and lights contained in LeftPanel
    QListWidget *objects, *lights;
    
    // Array of different cameras
    std::vector<ICamera*> camera;
    std::vector<QString> cameraText;
    CameraType currentCamera;
    
    // List of relations between objects, cameras and lights
    std::vector< std::pair<Object3D*, Light*> > objectRelations;
    std::vector< std::pair<ICamera*, Light*> > cameraRelations;
    
    // Settings
    Settings settings;
};

#endif	/* ENGINE_H */
