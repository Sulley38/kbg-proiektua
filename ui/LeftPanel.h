/*
 * Ezkerreko aukerak: objektu eta argien listak, eta kamera aukerak.
 */

#ifndef LEFTPANEL_H
#define	LEFTPANEL_H

#include <QComboBox>
#include <QLabel>
#include <QListWidget>
#include <QSlider>
#include <QWidget>

class LeftPanel : public QWidget {
    Q_OBJECT
    
public:
    LeftPanel(QWidget *parent = 0);
    
    QListWidget* getListObjects() const { return listObjects; }
    QLabel* getObjectPosition() const { return textObjectPosition; }
    QComboBox* getComboCameras() const { return comboCameras; }
    QSlider* getSliderZoom() const { return sliderZoom; }
    QLabel* getCameraPosition() const { return textCameraPosition; }
    QListWidget* getListLights() const { return listLights; }
    QLabel* getLightPosition() const { return textLightPosition; }
    
private:
    QListWidget *listObjects;
    QLabel *textObjectPosition;
    QComboBox *comboCameras;
    QSlider *sliderZoom;
    QLabel *textCameraPosition;
    QListWidget *listLights;
    QLabel *textLightPosition;
};

#endif	/* LEFTPANEL_H */