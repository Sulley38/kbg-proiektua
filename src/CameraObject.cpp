#include <vector>
#include "CameraObject.h"

CameraObject::CameraObject() :
    Object3D("./Objektuak/camera.obj", -1, true)
{
    // Make the origin the starting point for the camera
    // This way the camera's reference system will be the same as the object's one
    eye = Array3(0.0, 0.0, 0.0);
    at = Array3(0.0, 0.0, -10.0);
    
    // Change camera's position to the desired initial position
    move(0.0, 5.0, 10.0);
}

void CameraObject::move(double x, double y, double z, bool objectReference) {
    // Update object
    Object3D::move(x, y, z, objectReference);
    // Update camera position
    updatePosition();
}

void CameraObject::scale(double, double, double, bool) {
    // Do nothing: camera cannot be scaled
}

void CameraObject::rotate(Axis axis, int angle, bool objectReference) {
    // Update object
    Object3D::rotate(axis, angle, objectReference);
    // Update camera position
    updatePosition();
}

void CameraObject::undo() {
    // Update object
    Object3D::undo();
    // Update camera position
    updatePosition();
}

void CameraObject::redo() {
    // Update object
    Object3D::redo();
    // Update camera position
    updatePosition();
}

bool CameraObject::move(double x, double y, double z) {
    // Just like moving the object!
    move(x, y, z, true);
    return true;
}

bool CameraObject::rotate(Axis axis, int angle) {
    // Just like rotating the object!
    rotate(axis, angle, true);
    return true;
}

void CameraObject::reset() {
    // Back to the original position
    Object3D::reset();
    move(0.0, 5.0, 10.0);
    fovy = 60;
}

void CameraObject::updatePosition() {
    // Multiply the original camera coordinates with the transformation matrix
    const std::vector<double>& m = Object3D::tranformationMatrix();
    eye.v[0] = m[12],  eye.v[1] = m[13],  eye.v[2] = m[14];
    at.v[0] = m[12] - 10 * m[8], at.v[1] = m[13] - 10 * m[9], at.v[2] = m[14] - 10 * m[10];
    up.v[0] = m[4],  up.v[1] = m[5],  up.v[2] = m[6];
}