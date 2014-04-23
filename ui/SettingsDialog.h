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
 * Hobespenen elkarrizketa-koadroa.
 */

#ifndef SETTINGSDIALOG_H
#define	SETTINGSDIALOG_H

#include <QCheckBox>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QWidget>
#include "Definitions.h"

class SettingsDialog : public QDialog {
    Q_OBJECT
    
public:
    SettingsDialog(QWidget *parent = 0);
    
    // Load the current settings into the widgets of the dialog
    void load(const Settings& s);
    // Return the selected options and values
    const Settings& getSettings() const { return settings; }

private slots:
    void accept();
    
    void changeBgColor();
    void changeSelectColor();
    void changeCameraColor();
    void changeObjectColor();
    void changeGridColor();
    
private:
    Settings settings;
    
    QCheckBox *toggleGrid;
    QSpinBox *gridSize, *gridStep;
    QCheckBox *toggleQuadView, *toggleSightLine;
    QCheckBox *toggleLights;
    QLineEdit *bgColorEdit, *selectColorEdit, *cameraColorEdit, *objectColorEdit, *gridColorEdit;
    QPushButton *bgColorButton, *selectColorButton, *cameraColorButton, *objectColorButton, *gridColorButton;
    QPushButton *okButton;
};

#endif	/* SETTINGSDIALOG_H */
