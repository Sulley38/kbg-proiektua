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