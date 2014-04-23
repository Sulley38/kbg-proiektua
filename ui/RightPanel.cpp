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
#include <QGridLayout>
#include <QFormLayout>
#include <QLabel>
#include "RightPanel.h"

RightPanel::RightPanel(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *panelLayout = new QVBoxLayout(this);
    
    /** Title **/
    QLabel *lblTitle = new QLabel("<b>Transformazioak</b>");
    lblTitle->setAlignment(Qt::AlignHCenter);
    panelLayout->addWidget(lblTitle, 1, Qt::AlignVCenter);
    
    /** Target and operation combo boxes **/
    QFormLayout *optionsLayout = new QFormLayout();
    optionsLayout->setAlignment(Qt::AlignVCenter);
    comboTarget = new QComboBox();
    comboTarget->addItem("Objektua");
    comboTarget->addItem("Kamera");
    comboTarget->addItem("Argia");
    optionsLayout->addRow("Helburua:", comboTarget);
    comboOperation = new QComboBox();
    comboOperation->addItem("Mugitu");
    comboOperation->addItem("Tamaina aldatu");
    comboOperation->addItem("Biratu");
    optionsLayout->addRow("Aldaketa:", comboOperation);
    panelLayout->addLayout(optionsLayout, 2);
    
    /** Reference system check **/
    checkRefSystem = new QCheckBox("Objektuaren ES");
    checkRefSystem->setShortcut(QKeySequence(Qt::Key_K));
    panelLayout->addWidget(checkRefSystem, 1, Qt::AlignVCenter);
    
    /** Spin box selections **/
    stackedWidget = new QStackedWidget();
    stackedWidget->addWidget(initMovePage());
    stackedWidget->addWidget(initScalePage());
    stackedWidget->addWidget(initRotatePage());
    panelLayout->addWidget(stackedWidget, 3, Qt::AlignVCenter);
    
    /** Buttons **/
    QGridLayout *buttonLayout = new QGridLayout();
    buttonLayout->setAlignment(Qt::AlignVCenter);
    
    btnXNeg = new QPushButton("X-");
    btnXNeg->setShortcut(QKeySequence(Qt::Key_Left));
    buttonLayout->addWidget(btnXNeg, 0, 0);
    btnXPos = new QPushButton("X+");
    btnXPos->setShortcut(QKeySequence(Qt::Key_Right));
    buttonLayout->addWidget(btnXPos, 0, 1);
    btnYNeg = new QPushButton("Y-");
    btnYNeg->setShortcut(QKeySequence(Qt::Key_Down));
    buttonLayout->addWidget(btnYNeg, 1, 0);
    btnYPos = new QPushButton("Y+");
    btnYPos->setShortcut(QKeySequence(Qt::Key_Up));
    buttonLayout->addWidget(btnYPos, 1, 1);
    btnZNeg = new QPushButton("Z-");
    btnZNeg->setShortcut(QKeySequence(Qt::Key_PageUp));
    buttonLayout->addWidget(btnZNeg, 2, 0);
    btnZPos = new QPushButton("Z+");
    btnZPos->setShortcut(QKeySequence(Qt::Key_PageDown));
    buttonLayout->addWidget(btnZPos, 2, 1);
    btnAllNeg = new QPushButton("Denak-");
    btnAllNeg->setEnabled(false);
    btnAllNeg->setShortcut(QKeySequence(Qt::Key_Comma));
    buttonLayout->addWidget(btnAllNeg, 3, 0);
    btnAllPos = new QPushButton("Denak+");
    btnAllPos->setEnabled(false);
    btnAllPos->setShortcut(QKeySequence(Qt::Key_Period));
    buttonLayout->addWidget(btnAllPos, 3, 1);
    panelLayout->addLayout(buttonLayout, 4);
    
    /** Reset button **/
    btnReset =  new QPushButton("Berrezarri");
    btnReset->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
    panelLayout->addWidget(btnReset, 1, Qt::AlignVCenter);
}

QWidget* RightPanel::initMovePage() {
    QWidget *movePage = new QWidget();
    QFormLayout *layout = new QFormLayout(movePage);
    spinMoveDistanceX = new QDoubleSpinBox();
    spinMoveDistanceX->setRange(1.00, 50.0);
    spinMoveDistanceX->setSingleStep(0.01);
    spinMoveDistanceX->setValue(2.0);
    layout->addRow("X:", spinMoveDistanceX);
    spinMoveDistanceY = new QDoubleSpinBox();
    spinMoveDistanceY->setRange(1.00, 50.0);
    spinMoveDistanceY->setSingleStep(0.01);
    spinMoveDistanceY->setValue(2.0);
    layout->addRow("Y:", spinMoveDistanceY);
    spinMoveDistanceZ = new QDoubleSpinBox();
    spinMoveDistanceZ->setRange(1.00, 50.0);
    spinMoveDistanceZ->setSingleStep(0.01);
    spinMoveDistanceZ->setValue(2.0);
    layout->addRow("Z:", spinMoveDistanceZ);
    return movePage;
}

QWidget* RightPanel::initScalePage() {
    QWidget *scalePage = new QWidget();
    QFormLayout *layout = new QFormLayout(scalePage);
    spinScaleFactorX = new QDoubleSpinBox();
    spinScaleFactorX->setRange(1.00, 50.0);
    spinScaleFactorX->setSingleStep(0.01);
    spinScaleFactorX->setValue(2.0);
    layout->addRow("X:", spinScaleFactorX);
    spinScaleFactorY = new QDoubleSpinBox();
    spinScaleFactorY->setRange(1.00, 50.0);
    spinScaleFactorY->setSingleStep(0.01);
    spinScaleFactorY->setValue(2.0);
    layout->addRow("Y:", spinScaleFactorY);
    spinScaleFactorZ = new QDoubleSpinBox();
    spinScaleFactorZ->setRange(1.00, 50.0);
    spinScaleFactorZ->setSingleStep(0.01);
    spinScaleFactorZ->setValue(2.0);
    layout->addRow("Z:", spinScaleFactorZ);
    return scalePage;
}

QWidget* RightPanel::initRotatePage() {
    QWidget *rotatePage = new QWidget();
    QFormLayout *layout = new QFormLayout(rotatePage);
    spinRotateAngleX = new QSpinBox();
    spinRotateAngleX->setRange(1, 90);
    spinRotateAngleX->setSuffix(" \x00B0");
    spinRotateAngleX->setValue(15);
    layout->addRow("X:", spinRotateAngleX);
    spinRotateAngleY = new QSpinBox();
    spinRotateAngleY->setRange(1, 90);
    spinRotateAngleY->setSuffix(" \x00B0");
    spinRotateAngleY->setValue(15);
    layout->addRow("Y:", spinRotateAngleY);
    spinRotateAngleZ = new QSpinBox();
    spinRotateAngleZ->setRange(1, 90);
    spinRotateAngleZ->setSuffix(" \x00B0");
    spinRotateAngleZ->setValue(15);
    layout->addRow("Z:", spinRotateAngleZ);
    return rotatePage;
}
