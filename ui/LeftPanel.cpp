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

#include <QBoxLayout>
#include <QGroupBox>
#include "LeftPanel.h"

LeftPanel::LeftPanel(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *panelLayout = new QVBoxLayout(this);
    
    /** Objects group **/
    QGroupBox *groupObjects = new QGroupBox("Objektuak");
    QGridLayout *objectsLayout = new QGridLayout(groupObjects);
    
    listObjects = new QListWidget();
    objectsLayout->addWidget(listObjects, 0, 0);
    
    textObjectPosition = new QLabel();
    textObjectPosition->setMaximumWidth(180);
    objectsLayout->addWidget(textObjectPosition, 1, 0);
    
    panelLayout->addWidget(groupObjects);
    
    /** Camera group **/
    QGroupBox *groupCamera = new QGroupBox("Erabiltzaile-kamera");
    QGridLayout *cameraLayout = new QGridLayout(groupCamera);
    
    comboCameras = new QComboBox();
    comboCameras->addItem("Ortografikoa");
    comboCameras->addItem("Helikopteroa");
    comboCameras->addItem("Kamera-objektua");
    comboCameras->addItem("Ibiltaria");
    comboCameras->setCurrentIndex(1);
    cameraLayout->addWidget(comboCameras, 0, 0);
    
    QLabel *lblCameraZoom = new QLabel("Zoom egin:");
    cameraLayout->addWidget(lblCameraZoom, 1, 0);
    
    sliderZoom = new QSlider(Qt::Horizontal);
    sliderZoom->setRange(5, 15);
    sliderZoom->setValue(10);
    cameraLayout->addWidget(sliderZoom, 2, 0);
    
    textCameraPosition = new QLabel();
    textCameraPosition->setMaximumWidth(180);
    cameraLayout->addWidget(textCameraPosition, 3, 0);
    
    panelLayout->addWidget(groupCamera);
    
    /** Lights group **/
    QGroupBox *groupLights = new QGroupBox("Argiak");
    QGridLayout *lightsLayout = new QGridLayout(groupLights);
    
    listLights = new QListWidget();
    listLights->setSortingEnabled(true);
    lightsLayout->addWidget(listLights, 0, 0);
    
    textLightPosition = new QLabel();
    textLightPosition->setMaximumWidth(180);
    lightsLayout->addWidget(textLightPosition, 1, 0);
    
    groupLights->setMaximumHeight(220);
    panelLayout->addWidget(groupLights);
}
