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