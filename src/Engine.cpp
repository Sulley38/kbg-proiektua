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

#include <GL/glu.h>
#include <QtOpenGL>
#include "Camera2D.h"
#include "CameraFirstPerson.h"
#include "CameraHelicopter.h"
#include "CameraObject.h"
#include "Engine.h"

Engine::Engine(QListWidget *objectList, QListWidget *lightList, QWidget *parent) :
    QGLWidget(parent),
    // Save the references to the lists
    objects(objectList), lights(lightList),
    // Initialize vector of cameras and set current camera to ortographic
    camera(4), cameraText(4), currentCamera(Helicopter),
    // Initialize vectors of object/camera-light relationships
    objectRelations(), cameraRelations()
{
    // Activate OpenGL context
    makeCurrent();
    // Create the cameras
    camera[Ortho] = new Camera2D();
    cameraText[Ortho] = "K. Ortografikoa";
    camera[Helicopter] = new CameraHelicopter();
    cameraText[Helicopter] = "Helikoptero K.";
    camera[ObjectCam] = new CameraObject();
    cameraText[ObjectCam] = "Kamera-objektua";
    camera[FirstPerson] = new CameraFirstPerson();
    cameraText[FirstPerson] = "K. Ibiltaria";
    // Add object camera to the list
    objects->addItem(static_cast<CameraObject*>(camera[ObjectCam]));
    // Add an initial sun light 
    lights->addItem(new Light(Sun, 0));
    // Initial settings values
    settings.quadView = false;
    settings.lightsEnabled = true;
    settings.showGrid = true;
    settings.showSightLine = true;
    settings.gridSize = 50;
    settings.gridStep = 3;
    settings.bgColor = QColor::fromRgbF(0.5, 0.5, 0.5);
    settings.selectedColor = QColor::fromRgbF(1.0, 0.0, 0.0);
    settings.cameraColor = QColor::fromRgbF(0.25, 0.75, 1.0);
    settings.objectColor = QColor::fromRgbF(1.0, 1.0, 1.0);
    settings.gridColor = QColor::fromRgbF(0.1, 0.35, 0.1);
}

Engine::~Engine() {
    delete camera[Ortho];
    delete camera[Helicopter];
    // ObjectCam is deleted by Qt system because it's QListWidgetItem's child
    delete camera[FirstPerson];
}

void Engine::createRelation(Light* light, Object3D* object) {
    light->setRelation(objectRelations.size());
    if (object != 0) {
        // Relationship between light and object
        object->setRelation(objectRelations.size());
        light->setText(light->text() + " <-> " + object->text());
        // Move the light to the position of the object
        light->setPosition3(object->getPosition3());
        // Insert to the relationships table
        objectRelations.push_back(std::make_pair(object, light));
        cameraRelations.push_back(std::make_pair((ICamera*)NULL, (Light*)NULL));
    } else {
        // Relationship between light and camera
        camera[currentCamera]->setRelation(cameraRelations.size());
        light->setText(light->text() + " <-> " + cameraText[currentCamera]);
        // Move the light to the position of the camera
        light->followCamera(camera[currentCamera]->getEye(), camera[currentCamera]->getAt());
        // Insert to the relationships table
        objectRelations.push_back(std::make_pair((Object3D*)NULL, (Light*)NULL));
        cameraRelations.push_back(std::make_pair(camera[currentCamera], light));
    }
}

void Engine::removeRelation(const int rel) {
    // Remove relationship indices
    if (objectRelations[rel].first != NULL) {
        objectRelations[rel].first->setRelation(-1);
        objectRelations[rel].second->reset();
        objectRelations[rel].second->setRelation(-1);
        objectRelations[rel].second->setText(objectRelations[rel].second->text().split(" <-> ").first());
    } else {
        cameraRelations[rel].first->setRelation(-1);
        cameraRelations[rel].second->reset();
        cameraRelations[rel].second->setRelation(-1);
        cameraRelations[rel].second->setText(cameraRelations[rel].second->text().split(" <-> ").first());
    }
}

void Engine::setSettings(const Settings& s) {
    settings = s;
    glClearColor(s.bgColor.redF(), s.bgColor.greenF(), s.bgColor.blueF(), s.bgColor.alphaF());
}

void Engine::topView(float width, float height) {
    // Set projection
    camera[Ortho]->project(width / height);
    // Set camera position
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 1, 0, 0, 0, 0, 0, 0, -1);
    // Draw the scene
    drawScene();
}

void Engine::frontView(float width, float height) {
    // Set projection
    camera[Ortho]->project(width / height);
    // Set camera position
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 5, 1, 0, 5, 0, 0, 1, 0);
    // Draw the scene
    drawScene();
}

void Engine::sideView(float width, float height) {
    // Set projection
    camera[Ortho]->project(width / height);
    // Set camera position
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(1, 0, 0, 0, 0, 0, 0, 1, 0);
    // Draw the scene
    drawScene();
}

void Engine::userView(float width, float height) {
    // Set the projection matrix
    camera[currentCamera]->project(width / height);
    // Set the position of the camera
    Array3 eye = camera[currentCamera]->getEye(), at = camera[currentCamera]->getAt(), up = camera[currentCamera]->getUp();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye.v[0], eye.v[1], eye.v[2], at.v[0], at.v[1], at.v[2], up.v[0], up.v[1], up.v[2]);
    // Draw the scene, not showing the object camera if it's the current one
    drawScene(currentCamera != ObjectCam);
}

void Engine::drawScene(const bool showObjectCamera) {
    // Draw the background grid
    if (settings.showGrid)
        drawGrid();
        
    // Draw the line of sight of the camera object
    if (settings.showSightLine && showObjectCamera)
        drawSightLine();
    
    // Lighting system
    if (settings.lightsEnabled) {
        glEnable(GL_LIGHTING);
        // Draw the lights
        for (int i = 0; i < lights->count(); ++i) {
            glPushMatrix();
            static_cast<Light*>(lights->item(i))->draw(settings.lightsEnabled, settings.objectColor);
            glPopMatrix();
        }
    } else {
        glDisable(GL_LIGHTING);
    }
    
    // Draw the objects
    for (int i = (showObjectCamera) ? 0 : 1; i < objects->count(); ++i) {
        glPushMatrix();
        if (objects->currentRow() == i)
            static_cast<Object3D*>(objects->item(i))->draw(settings.lightsEnabled, settings.selectedColor, true);
        else
            static_cast<Object3D*>(objects->item(i))->draw(settings.lightsEnabled, (i == 0) ? settings.cameraColor : settings.objectColor);
        glPopMatrix();
    }
}

void Engine::drawGrid() {
    // Draw a grid on the XZ plane based on the settings using GL_LINES and no lighting
    glDisable(GL_LIGHTING);
    glColor3f(settings.gridColor.redF(), settings.gridColor.greenF(), settings.gridColor.blueF());
    glBegin(GL_LINES);
    for (int i = -settings.gridSize; i <= settings.gridSize; i += settings.gridStep) {
        glVertex3f(i, 0.0f, -settings.gridSize);
        glVertex3f(i, 0.0f, settings.gridSize);
        glVertex3f(-settings.gridSize, 0.0f, i);
        glVertex3f(settings.gridSize, 0.0f, i);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void Engine::drawSightLine() {
    // Draw the line between the camera and the point where it is looking
    Array3 eye = camera[ObjectCam]->getEye(), at = camera[ObjectCam]->getAt();
    glDisable(GL_LIGHTING);
    glColor3f(settings.cameraColor.redF(), settings.cameraColor.greenF(), settings.cameraColor.blueF());
    glLineStipple(3, 0x5555);
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
        glVertex3fv(eye.v);
        glVertex3fv(at.v);
    glEnd();
    glDisable(GL_LINE_STIPPLE);
    glEnable(GL_LIGHTING);
}

void Engine::initializeGL() {
    // Enable Z-Buffer and set material mode
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Set selected items on the lists
    objects->setCurrentRow(0);
    lights->setCurrentRow(0);
    // Set background color
    glClearColor(settings.bgColor.redF(), settings.bgColor.greenF(),
                 settings.bgColor.blueF(), settings.bgColor.alphaF());
}

void Engine::paintGL() {
    // Clear frame- and Z-buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (settings.quadView) {
        // Top view
        glViewport(0, height() / 2, width() / 2, height() / 2);
        topView(width() / 2, height() / 2);
        // Front view
        glViewport(0, 0, width() / 2, height() / 2);
        frontView(width() / 2, height() / 2);
        // Side view
        glViewport(width() / 2, height() / 2, width() / 2, height() / 2);
        sideView(width() / 2, height() / 2);
        // User view
        glViewport(width() / 2, 0, width() / 2, height() / 2);
        userView(width() / 2, height() / 2);
    } else {
        // Only an user view
        glViewport(0, 0, width(), height());
        userView(width(), height());
    }
    
    // Flush the rendering pipeline
    glFlush();
}

void Engine::wheelEvent(QWheelEvent *event) {
    // Emit proper zoom signals
    if (event->delta() > 0)
        emit zoom(1);
    else if (event->delta() < 0)
        emit zoom(-1);
    else
        event->ignore();
}
