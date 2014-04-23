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
 * Aplikazioaren leiho nagusia.
 * Interfaze osoa kontrolatzen du.
 */

#ifndef WINDOW_H
#define	WINDOW_H

#include <QAction>
#include <QActionGroup>
#include <QMainWindow>
#include "Definitions.h"
#include "Engine.h"
#include "LeftPanel.h"
#include "Light.h"
#include "Object3D.h"
#include "RightPanel.h"
#include "SettingsDialog.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
    
public:
    MainWindow();
    
protected:
    // Set user interaction commands (triggered by buttons, menus, etc.)
    void createActions();
    // Load the menu bar and its components
    void createMenus();
    // Establish connections between signals and slots
    void createConnections();
    
    // Get the selected object
    Object3D* currentObject() const { return static_cast<Object3D*>(leftPanel->getListObjects()->currentItem()); }
    // Get the selected light
    Light* currentLight() const { return static_cast<Light*>(leftPanel->getListLights()->currentItem()); }
    // Get the target of the transformations
    Target target() const { return static_cast<Target>(rightPanel->getComboTarget()->currentIndex()); }
    // Get the type of transformation selected
    Operation operation() const { return static_cast<Operation>(rightPanel->getComboOperation()->currentIndex()); }
    
private slots:
    /* Actions from GUI interaction (buttons, etc.) */
    // Object management
    void loadObject();
    void removeObject();
    // Materials
    void loadMaterial();
    void clearMaterial();
    // Edit actions
    void undo();
    void redo();
    void targetChange(int index);
    void targetChange(QAction* action);
    void operationChange(int index);
    void operationChange(QAction* action);
    // Change the program settings
    void settings();
    // Apply transformations
    void transformPositiveX();
    void transformNegativeX();
    void transformPositiveY();
    void transformNegativeY();
    void transformPositiveZ();
    void transformNegativeZ();
    void transformPositiveAll();
    void transformNegativeAll();
    void reset();
    // Update position texts on the left panel and enable/disable undo and redo menus
    void updateStatus();
    // Camera selection and adjustment
    void cameraChange(int index);
    void cameraChange(QAction* action);
    void nextCamera();
    void cameraZoomChange(int factor);
    void cameraLookAtObject();
    // Lighting system
    void addSun();
    void addLightBulb();
    void addSpotlight();
    void loadLight();
    void removeLight();
    void toggleLight();
    void changeLightType();
    void assignLightToObject();
    void assignLightToCamera();
    void unassignLight();
    // Help
    void showHelp();
    
private:
    // User interface actions
    QAction *actOpen, *actRemove, *actLoadMaterial, *actClearMaterial, *actExit;
    QAction *actUndo, *actRedo,
            *actTargetObject, *actTargetCamera, *actTargetLight,
            *actOperationMove, *actOperationScale, *actOperationRotate, 
            *actSettings;
    QAction *actQuadView,
            *actCameraOrtho, *actCameraFirstPerson, *actCameraHelicopter, *actCameraObject, *actNextCamera,
            *actZoomIn, *actZoomOut, *actLookAtObject;
    QAction *actToggleLighting, *actAddSun, *actAddLightbulb, *actAddSpotlight,
            *actLoadLight, *actRemoveLight, *actToggleLight, *actChangeType,
            *actRelateToObject, *actRelateToCamera, *actUnrelate;
    QAction *actUserGuide;
    QActionGroup *targetActions, *operationActions, *cameraActions;
    
    // Main widgets
    LeftPanel *leftPanel;
    Engine *glEngine;
    RightPanel *rightPanel;
    
    // Settings dialog
    SettingsDialog *settingsDialog;
};

#endif	/* WINDOW_H */
