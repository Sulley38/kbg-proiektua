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
 * Eskuineko aukerak: aldaketei buruzko widget-ak.
 */

#ifndef RIGHTPANEL_H
#define	RIGHTPANEL_H

#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QSpinBox>
#include <QStackedWidget>
#include <QWidget>

class RightPanel : public QWidget {
    Q_OBJECT
    
public:
    RightPanel(QWidget *parent = 0);

    QComboBox* getComboTarget() const { return comboTarget; }
    QComboBox* getComboOperation() const { return comboOperation; }
    QCheckBox* getCheckRefSystem() const { return checkRefSystem; }
    QStackedWidget* getStackedWidget() const { return stackedWidget; }
    QDoubleSpinBox* getMoveDistanceX() const { return spinMoveDistanceX; }
    QDoubleSpinBox* getMoveDistanceY() const { return spinMoveDistanceY; }
    QDoubleSpinBox* getMoveDistanceZ() const { return spinMoveDistanceZ; }
    QDoubleSpinBox* getScaleFactorX() const { return spinScaleFactorX; }
    QDoubleSpinBox* getScaleFactorY() const { return spinScaleFactorY; }
    QDoubleSpinBox* getScaleFactorZ() const { return spinScaleFactorZ; }
    QSpinBox* getRotateAngleX() const { return spinRotateAngleX; }
    QSpinBox* getRotateAngleY() const { return spinRotateAngleY; }
    QSpinBox* getRotateAngleZ() const { return spinRotateAngleZ; }
    QPushButton* getBtnXNeg() const { return btnXNeg; }
    QPushButton* getBtnXPos() const { return btnXPos; }
    QPushButton* getBtnYNeg() const { return btnYNeg; }
    QPushButton* getBtnYPos() const { return btnYPos; }
    QPushButton* getBtnZNeg() const { return btnZNeg; }
    QPushButton* getBtnZPos() const { return btnZPos; }
    QPushButton* getBtnAllNeg() const { return btnAllNeg; }
    QPushButton* getBtnAllPos() const { return btnAllPos; }
    QPushButton* getBtnReset() const { return btnReset; }
    
protected:
    QWidget* initMovePage();
    QWidget* initScalePage();
    QWidget* initRotatePage();
    
private:
    QComboBox *comboTarget, *comboOperation;
    QCheckBox *checkRefSystem;
    
    QStackedWidget *stackedWidget;
    QDoubleSpinBox *spinMoveDistanceX, *spinMoveDistanceY, *spinMoveDistanceZ;
    QDoubleSpinBox *spinScaleFactorX, *spinScaleFactorY, *spinScaleFactorZ;
    QSpinBox *spinRotateAngleX, *spinRotateAngleY, *spinRotateAngleZ;
    
    QPushButton *btnXNeg, *btnXPos, *btnYNeg, *btnYPos, *btnZNeg, *btnZPos, *btnAllNeg, *btnAllPos;
    QPushButton *btnReset;
};

#endif	/* RIGHTPANEL_H */
