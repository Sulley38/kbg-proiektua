#include <QBoxLayout>
#include <QColorDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include "SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    
    // Grid options
    QGroupBox *grid = new QGroupBox("Sareta");
    layout->addWidget(grid);
    
    QGridLayout *gridLayout = new QGridLayout(grid);
    toggleGrid = new QCheckBox("Sareta erakutsi");
    gridLayout->addWidget(toggleGrid, 0, 0, 1, 2);
    QLabel *gridSizeLabel = new QLabel("Saretaren aldea:");
    gridLayout->addWidget(gridSizeLabel, 1, 0);
    gridSize = new QSpinBox();
    gridSize->setRange(10, 1000);
    gridLayout->addWidget(gridSize, 1, 1);
    QLabel *gridStepLabel = new QLabel("Marra arteko tartea:");
    gridLayout->addWidget(gridStepLabel, 2, 0);
    gridStep = new QSpinBox();
    gridStep->setRange(1, 100);
    gridLayout->addWidget(gridStep, 2, 1);
    
    // Camera options
    QGroupBox *camera = new QGroupBox("Kamera");
    layout->addWidget(camera);
    
    QGridLayout *cameraLayout = new QGridLayout(camera);
    toggleQuadView = new QCheckBox("Lau ikuspuntu erakutsi");
    cameraLayout->addWidget(toggleQuadView, 0, 0);
    toggleSightLine = new QCheckBox("Objektu-kamerak nora apuntatzen duen erakutsi");
    cameraLayout->addWidget(toggleSightLine, 1, 0);
    
    // Light options
    QGroupBox *lights = new QGroupBox("Argiztatze-sistema");
    layout->addWidget(lights);
    
    QGridLayout *lightsLayout = new QGridLayout(lights);
    toggleLights = new QCheckBox("Argiak erabili");
    lightsLayout->addWidget(toggleLights, 0, 0);
    
    // Color options
    QGroupBox *colors = new QGroupBox("Koloreak");
    layout->addWidget(colors);
    
    QGridLayout *colorsLayout = new QGridLayout(colors);
    QLabel *bgColorLabel = new QLabel("Atzeko planoa:");
    colorsLayout->addWidget(bgColorLabel, 0, 0);
    QLabel *selectColorLabel = new QLabel("Aukeratutako objektua:");
    colorsLayout->addWidget(selectColorLabel, 1, 0);
    QLabel *cameraColorLabel = new QLabel("Kamera-objektua:");
    colorsLayout->addWidget(cameraColorLabel, 2, 0);
    QLabel *objectColorLabel = new QLabel("Gainerako objektuak:");
    colorsLayout->addWidget(objectColorLabel, 3, 0);
    QLabel *gridColorLabel = new QLabel("Sareta:");
    colorsLayout->addWidget(gridColorLabel, 4, 0);
    
    bgColorEdit = new QLineEdit();
    bgColorEdit->setAlignment(Qt::AlignHCenter);
    bgColorEdit->setReadOnly(true);
    colorsLayout->addWidget(bgColorEdit, 0, 1);
    selectColorEdit = new QLineEdit();
    selectColorEdit->setAlignment(Qt::AlignHCenter);
    selectColorEdit->setReadOnly(true);
    colorsLayout->addWidget(selectColorEdit, 1, 1);
    cameraColorEdit = new QLineEdit();
    cameraColorEdit->setAlignment(Qt::AlignHCenter);
    cameraColorEdit->setReadOnly(true);
    colorsLayout->addWidget(cameraColorEdit, 2, 1);
    objectColorEdit = new QLineEdit();
    objectColorEdit->setAlignment(Qt::AlignHCenter);
    objectColorEdit->setReadOnly(true);
    colorsLayout->addWidget(objectColorEdit, 3, 1);
    gridColorEdit = new QLineEdit();
    gridColorEdit->setAlignment(Qt::AlignHCenter);
    gridColorEdit->setReadOnly(true);
    colorsLayout->addWidget(gridColorEdit, 4, 1);
    
    bgColorButton = new QPushButton("Aldatu");
    colorsLayout->addWidget(bgColorButton, 0, 2);
    selectColorButton = new QPushButton("Aldatu");
    colorsLayout->addWidget(selectColorButton, 1, 2);
    cameraColorButton = new QPushButton("Aldatu");
    colorsLayout->addWidget(cameraColorButton, 2, 2);
    objectColorButton = new QPushButton("Aldatu");
    colorsLayout->addWidget(objectColorButton, 3, 2);
    gridColorButton = new QPushButton("Aldatu");
    colorsLayout->addWidget(gridColorButton, 4, 2);
    
    // Accept button
    okButton = new QPushButton("Ados");
    okButton->setDefault(true);
    okButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    layout->addWidget(okButton, 0, Qt::AlignHCenter);
    
    // Make connections
    connect(bgColorButton, SIGNAL(clicked()), this, SLOT(changeBgColor()));
    connect(selectColorButton, SIGNAL(clicked()), this, SLOT(changeSelectColor()));
    connect(cameraColorButton, SIGNAL(clicked()), this, SLOT(changeCameraColor()));
    connect(objectColorButton, SIGNAL(clicked()), this, SLOT(changeObjectColor()));
    connect(gridColorButton, SIGNAL(clicked()), this, SLOT(changeGridColor()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
}

void SettingsDialog::load(const Settings& s) {
    settings = s;
    // Modify widgets according to current settings
    toggleGrid->setChecked(s.showGrid);
    gridSize->setValue(s.gridSize);
    gridStep->setValue(s.gridStep);
    toggleQuadView->setChecked(s.quadView);
    toggleSightLine->setChecked(s.showSightLine);
    toggleLights->setChecked(s.lightsEnabled);
    bgColorEdit->setStyleSheet("background: " + s.bgColor.name());
    bgColorEdit->setText(s.bgColor.name());
    selectColorEdit->setStyleSheet("background: " + s.selectedColor.name());
    selectColorEdit->setText(s.selectedColor.name());
    cameraColorEdit->setStyleSheet("background: " + s.cameraColor.name());
    cameraColorEdit->setText(s.cameraColor.name());
    objectColorEdit->setStyleSheet("background: " + s.objectColor.name());
    objectColorEdit->setText(s.objectColor.name());
    gridColorEdit->setStyleSheet("background: " + s.gridColor.name());
    gridColorEdit->setText(s.gridColor.name());
}

void SettingsDialog::accept() {
    // Save selected values
    settings.showGrid = (toggleGrid->checkState() == Qt::Checked);
    settings.gridSize = gridSize->value();
    settings.gridStep = gridStep->value();
    settings.quadView = (toggleQuadView->checkState() == Qt::Checked);
    settings.showSightLine = (toggleSightLine->checkState() == Qt::Checked);
    settings.lightsEnabled = (toggleLights->checkState() == Qt::Checked);
    QDialog::accept();
}

void SettingsDialog::changeBgColor() {
    settings.bgColor = QColorDialog::getColor(settings.bgColor, this, "Kolorea aukeratu");
    bgColorEdit->setStyleSheet("background: " + settings.bgColor.name());
    bgColorEdit->setText(settings.bgColor.name());
}

void SettingsDialog::changeSelectColor() {
    settings.selectedColor = QColorDialog::getColor(settings.selectedColor, this, "Kolorea aukeratu");
    selectColorEdit->setStyleSheet("background: " + settings.selectedColor.name());
    selectColorEdit->setText(settings.selectedColor.name());
}

void SettingsDialog::changeCameraColor() {
    settings.cameraColor = QColorDialog::getColor(settings.cameraColor, this, "Kolorea aukeratu");
    cameraColorEdit->setStyleSheet("background: " + settings.cameraColor.name());
    cameraColorEdit->setText(settings.cameraColor.name());
}

void SettingsDialog::changeObjectColor() {
    settings.objectColor = QColorDialog::getColor(settings.objectColor, this, "Kolorea aukeratu");
    objectColorEdit->setStyleSheet("background: " + settings.objectColor.name());
    objectColorEdit->setText(settings.objectColor.name());
}

void SettingsDialog::changeGridColor() {
    settings.gridColor = QColorDialog::getColor(settings.gridColor, this, "Kolorea aukeratu");
    gridColorEdit->setStyleSheet("background: " + settings.gridColor.name());
    gridColorEdit->setText(settings.gridColor.name());
}
