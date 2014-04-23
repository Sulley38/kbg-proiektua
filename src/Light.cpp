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

#include <fstream>
#include <GL/gl.h>
#include "ICamera.h"
#include "Light.h"

Light::Light(const LightType T, const int lightNumber) :
    Object3D("./Objektuak/bulb.obj", lightNumber),
    type(Lightbulb), on(true),
    ambient(0.2, 0.2, 0.2, 1.0), diffuse(0.8, 0.8, 0.8, 1.0), specular(1.0, 1.0, 1.0, 1.0),
    spotDirection(0.0, 0.0, -1.0), spotExponent(0.0), spotCutoff(180.0),
    constantAtt(0.5), linearAtt(0.0), quadraticAtt(0.05) {
    switch (T) {
    case Sun:
        move(0.0, -getPosition().v[1], 1.0, true);
        changeType(); // To spotlight
        changeType(); // To sun
        break;
    case Lightbulb:
        loadMaterial("./Materialak/lightbulb.mtl");
        setText(QString("%1: Bonbilla").arg(getId()));
        break;
    case Spotlight:
        move(5.0, -getPosition().v[1] + 6, 0.0, true);
        changeType(); // To spotlight
        break;
    }
    setParameters();
    glEnable(GL_LIGHT0 + lightNumber);
}

Light::Light(const char* filename, const int lightNumber) :
    Object3D("./Objektuak/bulb.obj", lightNumber), on(true) {
    // Parse the material description file
    int lighttype;
    std::ifstream file(filename);
    file >> lighttype;
    file >> ambient.v[0] >> ambient.v[1] >> ambient.v[2] >> ambient.v[3];
    file >> diffuse.v[0] >> diffuse.v[1] >> diffuse.v[2] >> diffuse.v[3];
    file >> specular.v[0] >> specular.v[1] >> specular.v[2] >> specular.v[3];
    file >> spotDirection.v[0] >> spotDirection.v[1] >> spotDirection.v[2];
    file >> spotExponent >> spotCutoff;
    file >> constantAtt >> linearAtt >> quadraticAtt;
    file.close();
    // Set appropiate text of the object
    type = static_cast<LightType>(lighttype);
    Array4 position;
    switch (type) {
    case Sun:
        move(0.0, -getPosition().v[1], 1.0, true);
        position = getPosition();
        position.v[3] = 0.0;
        setPosition(position);
        setText(QString("%1: Eguzkia").arg(getId()));
        break;
    case Lightbulb:
        loadMaterial("./Materialak/lightbulb.mtl");
        setText(QString("%1: Bonbilla").arg(getId()));
        break;
    case Spotlight:
        load("./Objektuak/spotlight.obj");
        loadMaterial("./Materialak/black plastic.mtl");
        move(5.0, -getPosition().v[1] + 6, 0.0, true);
        setText(QString("%1: Fokua").arg(getId()));
        break;
    }
    setParameters();
    glEnable(GL_LIGHT0 + lightNumber);
}

void Light::draw(const bool lightsEnabled, const QColor& color, const bool boundingBox) {
    // Update the position of the light (already multiplied with the transformation matrix)
    glLightfv(GL_LIGHT0 + getId(), GL_POSITION, getPosition().v);
    // Draw the light if it isn't a sun and it isn't related to an object or camera
    if (type != Sun && getRelation() == -1)
        Object3D::draw(lightsEnabled, color, boundingBox);
    // Update the spotlight direction (it will get multiplied with the transformation matrix)
    if (type == Spotlight)
        glLightfv(GL_LIGHT0 + getId(), GL_SPOT_DIRECTION, spotDirection.v);
}

void Light::reset() {
    Object3D::reset();
    if (type == Spotlight)
        spotDirection = Array3(-0.5, -0.25, 0.0);
    else
        spotDirection = Array3(0.0, 0.0, -1.0);
}

void Light::followCamera(const Array3& eye, const Array3& at) {
    // Set as position the eye and as spot direction the sight of the camera
    setPosition3(eye);
    spotDirection.v[0] = at.v[0] - eye.v[0];
    spotDirection.v[1] = at.v[1] - eye.v[1];
    spotDirection.v[2] = at.v[2] - eye.v[2];
}

bool Light::toggle() {
    if (on) {
        glDisable(GL_LIGHT0 + getId());
        if (type == Lightbulb)
            loadMaterial("./Materialak/chrome.mtl");
    } else {
        glEnable(GL_LIGHT0 + getId());
        if (type == Lightbulb)
            loadMaterial("./Materialak/lightbulb.mtl");
    }
    return (on = !on);
}

void Light::changeType() {
    Array4 position;
    switch (type) {
    case Sun:
        type = Lightbulb;
        position = getPosition();
        position.v[3] = 1.0;
        setPosition(position);
        spotCutoff = 180.0;
        spotDirection = Array3(0.0, 0.0, -1.0);
        load("./Objektuak/bulb.obj");
        if (on)
            loadMaterial("./Materialak/lightbulb.mtl");
        else
            loadMaterial("./Materialak/chrome.mtl");
        setText(QString("%1: Bonbilla").arg(getId()));
        break;
    case Lightbulb:
        type = Spotlight;
        position = getPosition();
        position.v[3] = 1.0;
        setPosition(position);
        spotCutoff = 30.0;
        spotDirection = Array3(-0.5, -0.25, 0.0);
        load("./Objektuak/spotlight.obj");
        loadMaterial("./Materialak/black plastic.mtl");
        setText(QString("%1: Fokua").arg(getId()));
        break;
    case Spotlight:
        type = Sun;
        position = getPosition();
        position.v[3] = 0.0;
        setPosition(position);
        spotCutoff = 180.0;
        spotDirection = Array3(0.0, 0.0, -1.0);
        setText(QString("%1: Eguzkia").arg(getId()));
        break;
    }
    setParameters();
}

void Light::setParameters() {
    // Set OpenGL light parameters
    glLightfv(GL_LIGHT0 + getId(), GL_AMBIENT, ambient.v);
    glLightfv(GL_LIGHT0 + getId(), GL_DIFFUSE, diffuse.v);
    glLightfv(GL_LIGHT0 + getId(), GL_SPECULAR, specular.v);
    glLightf(GL_LIGHT0 + getId(), GL_SPOT_CUTOFF, spotCutoff);
    glLightf(GL_LIGHT0 + getId(), GL_SPOT_EXPONENT, spotExponent);
    glLightf(GL_LIGHT0 + getId(), GL_CONSTANT_ATTENUATION, constantAtt);
    glLightf(GL_LIGHT0 + getId(), GL_LINEAR_ATTENUATION, linearAtt);
    glLightf(GL_LIGHT0 + getId(), GL_QUADRATIC_ATTENUATION, quadraticAtt);
}
