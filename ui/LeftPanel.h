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
