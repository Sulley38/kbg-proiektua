#include <QApplication>
#include <QBoxLayout>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QMenuBar>
#include <QSignalMapper>
#include <QStatusBar>
#include <QStringList>
#include <QUrl>
#include <QWidget>
#include "ICamera.h"
#include "MainWindow.h"

MainWindow::MainWindow() : QMainWindow() {
    // Set window properties
    setGeometry((QApplication::desktop()->screenGeometry().width() / 2) - (WIDTH / 2), 
                (QApplication::desktop()->screenGeometry().height() / 2) - (HEIGHT / 2),
                WIDTH, HEIGHT);
    setMinimumSize(600, 400);
    setWindowTitle("KbG Proiektua");
    
    // Create a central widget
    QWidget *mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);
    
    // Create actions and menus
    createActions();
    createMenus();
    
    // Create and add child widgets to the central widget
    QHBoxLayout *mainLayout = new QHBoxLayout(mainWidget);
    leftPanel = new LeftPanel();
    leftPanel->setFixedWidth(220);
    mainLayout->addWidget(leftPanel);
    glEngine = new Engine(leftPanel->getListObjects(), leftPanel->getListLights());
    glEngine->setFocusPolicy(Qt::ClickFocus);
    mainLayout->addWidget(glEngine, 1);
    rightPanel = new RightPanel();
    rightPanel->setFixedWidth(220);
    mainLayout->addWidget(rightPanel);
    
    // Create the settings dialog
    settingsDialog = new SettingsDialog(this);
    
    // Set GUI signals' connections
    createConnections();
    
    // Set the permanent message on the status bar
    QLabel *labelReady = new QLabel(">> Prest");
    labelReady->setMargin(4);
    statusBar()->addWidget(labelReady);
}

void MainWindow::createActions() {
    /** File menu **/
    // Open file
    actOpen = new QAction("Objektua(k) kargatu", this);
    actOpen->setShortcut(QKeySequence(Qt::Key_L));
    // Remove object
    actRemove = new QAction("Objektua ezabatu", this);
    actRemove->setShortcut(QKeySequence(Qt::Key_Delete));
    // Load a material for the selected object
    actLoadMaterial = new QAction("Materiala kargatu", this);
    actLoadMaterial->setShortcut(QKeySequence(Qt::Key_T));
    // Remove the material for the selected object
    actClearMaterial = new QAction("Materiala ezabatu", this);
    actClearMaterial->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));
    // Exit
    actExit = new QAction("Irten", this);
    actExit->setShortcut(QKeySequence(Qt::Key_Escape));
    
    /** Edit menu **/
    // Undo
    actUndo = new QAction("Desegin", this);
    actUndo->setEnabled(false);
    actUndo->setShortcut(QKeySequence::Undo);
    // Redo
    actRedo = new QAction("Berregin", this);
    actRedo->setEnabled(false);
    actRedo->setShortcut(QKeySequence::Redo);
    // Target selection
    actTargetObject = new QAction("Objektua", this);
    actTargetObject->setCheckable(true);
    actTargetObject->setShortcut(QKeySequence(Qt::Key_O));
    actTargetCamera = new QAction("Kamera", this);
    actTargetCamera->setCheckable(true);
    actTargetCamera->setShortcut(QKeySequence(Qt::Key_C));
    actTargetLight = new QAction("Argia", this);
    actTargetLight->setCheckable(true);
    actTargetLight->setShortcut(QKeySequence(Qt::Key_G));
    targetActions = new QActionGroup(this);
    targetActions->addAction(actTargetObject);
    targetActions->addAction(actTargetCamera);
    targetActions->addAction(actTargetLight);
    actTargetObject->setChecked(true);
    // Operation selection
    actOperationMove = new QAction("Mugitu", this);
    actOperationMove->setCheckable(true);
    actOperationMove->setShortcut(QKeySequence(Qt::Key_M));
    actOperationScale = new QAction("Tamaina aldatu", this);
    actOperationScale->setCheckable(true);
    actOperationScale->setShortcut(QKeySequence(Qt::Key_S));
    actOperationRotate = new QAction("Biratu", this);
    actOperationRotate->setCheckable(true);
    actOperationRotate->setShortcut(QKeySequence(Qt::Key_R));
    operationActions = new QActionGroup(this);
    operationActions->addAction(actOperationMove);
    operationActions->addAction(actOperationScale);
    operationActions->addAction(actOperationRotate);
    actOperationMove->setChecked(true);
    // Open the settings dialog
    actSettings = new QAction("Hobespenak", this);

    /** View menu **/
    // Activate quad view
    actQuadView = new QAction("Lau ikuspuntu", this);
    actQuadView->setCheckable(true);
    actQuadView->setShortcut(QKeySequence(Qt::Key_Q));
    // Camera selection
    actCameraOrtho = new QAction("Ortografikoa", this);
    actCameraOrtho->setCheckable(true);
    actCameraOrtho->setShortcut(QKeySequence(Qt::Key_1));
    actCameraHelicopter = new QAction("Helikopteroa", this);
    actCameraHelicopter->setCheckable(true);
    actCameraHelicopter->setShortcut(QKeySequence(Qt::Key_2));
    actCameraObject = new QAction("Kamera-objektua", this);
    actCameraObject->setCheckable(true);
    actCameraObject->setShortcut(QKeySequence(Qt::Key_3));
    actCameraFirstPerson = new QAction("Ibiltaria", this);
    actCameraFirstPerson->setCheckable(true);
    actCameraFirstPerson->setShortcut(QKeySequence(Qt::Key_4));
    cameraActions = new QActionGroup(this);
    cameraActions->addAction(actCameraOrtho);
    cameraActions->addAction(actCameraHelicopter);
    cameraActions->addAction(actCameraObject);
    cameraActions->addAction(actCameraFirstPerson);
    actCameraHelicopter->setChecked(true);
    // Next camera
    actNextCamera = new QAction("Hurrengo kamera", this);
    actNextCamera->setShortcut(QKeySequence(Qt::Key_V));
    // Zoom in and out
    actZoomIn = new QAction("Hurbildu", this);
    actZoomIn->setShortcut(QKeySequence(Qt::Key_Plus));
    actZoomOut = new QAction("Urrundu", this);
    actZoomOut->setShortcut(QKeySequence(Qt::Key_Minus));
    // Adjust the viewpoint to the current object
    actLookAtObject = new QAction("Objektura begiratu", this);
    actLookAtObject->setShortcut(QKeySequence(Qt::Key_A));
    
    /** Lights menu **/
    // Lighting system toggle
    actToggleLighting = new QAction("Argiztatze-sistema erabili", this);
    actToggleLighting->setCheckable(true);
    actToggleLighting->setChecked(true);
    actToggleLighting->setShortcut(QKeySequence(Qt::Key_F12));
    // Add a default light
    actAddSun = new QAction("Eguzkia gehitu", this);
    actAddSun->setShortcut(QKeySequence(Qt::Key_F2));
    actAddLightbulb = new QAction("Bonbilla gehitu", this);
    actAddLightbulb->setShortcut(QKeySequence(Qt::Key_F3));
    actAddSpotlight = new QAction("Fokua gehitu", this);
    actAddSpotlight->setShortcut(QKeySequence(Qt::Key_F4));
    // Add a light from file
    actLoadLight = new QAction("Fitxategitik kargatu", this);
    actLoadLight->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    // Delete the selected light
    actRemoveLight = new QAction("Ezabatu", this);
    actRemoveLight->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Delete));
    // Toggle selected light on/off
    actToggleLight = new QAction("Piztu / Itzali", this);
    actToggleLight->setShortcut(QKeySequence(Qt::Key_F1));
    // Change to the next type of light
    actChangeType = new QAction("Mota aldatu", this);
    actChangeType->setShortcut(QKeySequence(Qt::Key_Space));
    // Assign a light to the selected object
    actRelateToObject = new QAction("Objektuari esleitu", this);
    actRelateToObject->setShortcut(QKeySequence(Qt::Key_F5));
    // Assign a light to the current camera
    actRelateToCamera = new QAction("Kamerari esleitu", this);
    actRelateToCamera->setShortcut(QKeySequence(Qt::Key_F6));
    // Assign a light to the current camera
    actUnrelate = new QAction("Esleipena askatu", this);
    actUnrelate->setShortcut(QKeySequence(Qt::Key_F7));
    
    /** Help menu **/
    // Show user guide
    actUserGuide = new QAction("Erabiltzaile gida", this);
    actUserGuide->setShortcut(QKeySequence(Qt::Key_E));
}

void MainWindow::createMenus() {
    QMenu *fileMenu = menuBar()->addMenu("Fitxategia");
    fileMenu->addAction(actOpen);
    fileMenu->addAction(actRemove);
    fileMenu->addSeparator();
    fileMenu->addAction(actLoadMaterial);
    fileMenu->addAction(actClearMaterial);
    fileMenu->addSeparator();
    fileMenu->addAction(actExit);
    
    QMenu *editMenu = menuBar()->addMenu("Editatu");
    editMenu->addAction(actUndo);
    editMenu->addAction(actRedo);
    editMenu->addSeparator()->setText("Helburua");
    editMenu->addAction(actTargetObject);
    editMenu->addAction(actTargetCamera);
    editMenu->addAction(actTargetLight);
    editMenu->addSeparator()->setText("Aldaketa");
    editMenu->addAction(actOperationMove);
    editMenu->addAction(actOperationScale);
    editMenu->addAction(actOperationRotate);
    editMenu->addSeparator();
    editMenu->addAction(actSettings);
    
    QMenu *viewMenu = menuBar()->addMenu("Kamera");
    viewMenu->addAction(actQuadView);
    viewMenu->addSeparator();
    viewMenu->addAction(actCameraOrtho);
    viewMenu->addAction(actCameraHelicopter);
    viewMenu->addAction(actCameraObject);
    viewMenu->addAction(actCameraFirstPerson);
    viewMenu->addSeparator();
    viewMenu->addAction(actNextCamera);
    viewMenu->addSeparator();
    viewMenu->addAction(actZoomIn);
    viewMenu->addAction(actZoomOut);
    viewMenu->addSeparator();
    viewMenu->addAction(actLookAtObject);
    
    QMenu *lightMenu = menuBar()->addMenu("Argia");
    lightMenu->addAction(actToggleLighting);
    lightMenu->addSeparator();
    QMenu *addLightMenu = lightMenu->addMenu("Argi lehenetsiak");
    addLightMenu->addAction(actAddSun);
    addLightMenu->addAction(actAddLightbulb);
    addLightMenu->addAction(actAddSpotlight);
    lightMenu->addAction(actLoadLight);
    lightMenu->addAction(actRemoveLight);
    lightMenu->addSeparator();
    lightMenu->addAction(actToggleLight);
    lightMenu->addAction(actChangeType);
    lightMenu->addSeparator();
    lightMenu->addAction(actRelateToObject);
    lightMenu->addAction(actRelateToCamera);
    lightMenu->addAction(actUnrelate);
    
    QMenu *helpMenu = menuBar()->addMenu("Laguntza");
    helpMenu->addAction(actUserGuide);
}

void MainWindow::createConnections() {
    // File menu
    connect(actOpen, SIGNAL(triggered()), this, SLOT(loadObject()));
    connect(actRemove, SIGNAL(triggered()), this, SLOT(removeObject()));
    connect(actLoadMaterial, SIGNAL(triggered()), this, SLOT(loadMaterial()));
    connect(actClearMaterial, SIGNAL(triggered()), this, SLOT(clearMaterial()));
    connect(actExit, SIGNAL(triggered()), this, SLOT(close()));
    // Edit menu
    connect(actUndo, SIGNAL(triggered()), this, SLOT(undo()));
    connect(actRedo, SIGNAL(triggered()), this, SLOT(redo()));
    connect(targetActions, SIGNAL(triggered(QAction*)), this, SLOT(targetChange(QAction*)));
    connect(operationActions, SIGNAL(triggered(QAction*)), this, SLOT(operationChange(QAction*)));
    connect(actSettings, SIGNAL(triggered()), this, SLOT(settings()));
    // View menu
    connect(actQuadView, SIGNAL(triggered()), glEngine, SLOT(toggleQuadView()));
    connect(cameraActions, SIGNAL(triggered(QAction*)), this, SLOT(cameraChange(QAction*)));
    connect(actNextCamera, SIGNAL(triggered()), this, SLOT(nextCamera()));
    QSignalMapper *signalMapper = new QSignalMapper(this);
    signalMapper->setMapping(actZoomIn, 1);
    signalMapper->setMapping(actZoomOut, -1);
    connect(actZoomIn, SIGNAL(triggered()), signalMapper, SLOT(map()));
    connect(actZoomOut, SIGNAL(triggered()), signalMapper, SLOT(map()));
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(cameraZoomChange(int)));
    connect(actLookAtObject, SIGNAL(triggered()), this, SLOT(cameraLookAtObject()));
    // Light menu
    connect(actToggleLighting, SIGNAL(triggered()), glEngine, SLOT(toggleLighting()));
    connect(actAddSun, SIGNAL(triggered()), this, SLOT(addSun()));
    connect(actAddLightbulb, SIGNAL(triggered()), this, SLOT(addLightBulb()));
    connect(actAddSpotlight, SIGNAL(triggered()), this, SLOT(addSpotlight()));
    connect(actLoadLight, SIGNAL(triggered()), this, SLOT(loadLight()));
    connect(actRemoveLight, SIGNAL(triggered()), this, SLOT(removeLight()));
    connect(actToggleLight, SIGNAL(triggered()), this, SLOT(toggleLight()));
    connect(actChangeType, SIGNAL(triggered()), this, SLOT(changeLightType()));
    connect(actRelateToObject, SIGNAL(triggered()), this, SLOT(assignLightToObject()));
    connect(actRelateToCamera, SIGNAL(triggered()), this, SLOT(assignLightToCamera()));
    connect(actUnrelate, SIGNAL(triggered()), this, SLOT(unassignLight()));
    // Help menu
    connect(actUserGuide, SIGNAL(triggered()), this, SLOT(showHelp()));
    // Left panel
    connect(leftPanel->getListObjects(), SIGNAL(itemSelectionChanged()), this, SLOT(updateStatus()));
    connect(leftPanel->getComboCameras(), SIGNAL(activated(int)), this, SLOT(cameraChange(int)));
    connect(leftPanel->getSliderZoom(), SIGNAL(valueChanged(int)), this, SLOT(cameraZoomChange(int)));
    connect(leftPanel->getListLights(), SIGNAL(itemSelectionChanged()), this, SLOT(updateStatus()));
    // OpenGL Widget
    connect(glEngine, SIGNAL(zoom(int)), this, SLOT(cameraZoomChange(int)));
    // Right panel
    connect(rightPanel->getComboTarget(), SIGNAL(activated(int)), this, SLOT(targetChange(int)));
    connect(rightPanel->getComboOperation(), SIGNAL(activated(int)), this, SLOT(operationChange(int)));
    connect(rightPanel->getBtnXNeg(), SIGNAL(clicked()), this, SLOT(transformNegativeX()));
    connect(rightPanel->getBtnXPos(), SIGNAL(clicked()), this, SLOT(transformPositiveX()));
    connect(rightPanel->getBtnYNeg(), SIGNAL(clicked()), this, SLOT(transformNegativeY()));
    connect(rightPanel->getBtnYPos(), SIGNAL(clicked()), this, SLOT(transformPositiveY()));
    connect(rightPanel->getBtnZNeg(), SIGNAL(clicked()), this, SLOT(transformNegativeZ()));
    connect(rightPanel->getBtnZPos(), SIGNAL(clicked()), this, SLOT(transformPositiveZ()));
    connect(rightPanel->getBtnAllNeg(), SIGNAL(clicked()), this, SLOT(transformNegativeAll()));
    connect(rightPanel->getBtnAllPos(), SIGNAL(clicked()), this, SLOT(transformPositiveAll()));
    connect(rightPanel->getBtnReset(), SIGNAL(clicked()), this, SLOT(reset()));
}

/******************** GUI SLOTS ******************/

void MainWindow::loadObject() {
    QStringList files = QFileDialog::getOpenFileNames(this, "Select one or more files to open", QString(), "Wavefront object files (*.obj);;All files (*.*)");
    // For each file, load the object and add it to the list
    int loaded = 0;
    for (QStringList::const_iterator it = files.constBegin(); it != files.constEnd(); ++it) {
        Object3D *object = new Object3D(*it);
        if (object->getId() >= 0) {
            // If it's a valid object, add it to the list and set it as selected
            leftPanel->getListObjects()->addItem(object);
            leftPanel->getListObjects()->setCurrentRow(leftPanel->getListObjects()->count() - 1);
            loaded++;
        } else {
            // If the object is invalid, delete it
            delete object;
        }
    }
    glEngine->updateGL();
    // Show message in the status bar
    if (loaded == 0)
        statusBar()->showMessage(">> Ez da objektu baliodunik aurkitu", 2000);
    else if (loaded == 1)
        statusBar()->showMessage(">> Objektu bat kargatu da", 2000);
    else
        statusBar()->showMessage(QString(">> %1 objektu kargatu dira").arg(loaded), 2000);
}

void MainWindow::removeObject() {
    if (currentObject()->getId() > 0) {
        // Remove the object
        delete leftPanel->getListObjects()->takeItem(leftPanel->getListObjects()->currentRow());
        glEngine->updateGL();
        statusBar()->showMessage(">> Objektua ezabatu da", 2000);
    } else {
         // Do not allow to remove the object camera
        statusBar()->showMessage(">> Ezin da kamera ezabatu", 2000);
    }
}

void MainWindow::loadMaterial() {
    // Set material for the current object
    QString file = QFileDialog::getOpenFileName(this, "Select one file to open", QString(), "Material description files (*.mtl);;All files (*.*)");
    if (!file.isEmpty()) {
        currentObject()->loadMaterial(file.toUtf8().constData());
        glEngine->updateGL();
        statusBar()->showMessage(">> Materiala kargatu da", 2000);
    }
}

void MainWindow::clearMaterial() {
    // Unset material for the current object
    if (currentObject()->hasMaterial()) {
        currentObject()->clearMaterial();
        glEngine->updateGL();
        statusBar()->showMessage(">> Materiala ezabatu da", 2000);
    } else {
        statusBar()->showMessage(">> Objektu honek ez du materialik", 2000);
    }
}

void MainWindow::undo() {
    if (target() == TObject) {
        // Undo object transformation
        currentObject()->undo();
        if (currentObject()->getRelation() != -1)
            glEngine->getRelatedLight(currentObject())->setPosition3(currentObject()->getPosition3());
    } else if (target() == TLight) {
        // Undo light transformation
        currentLight()->undo();
        if (currentLight()->getRelation() != -1)
            glEngine->removeRelation(currentLight()->getRelation());
    }
    updateStatus();
    statusBar()->showMessage(">> Aldaketa desegin da", 2000);
}

void MainWindow::redo() {
    if (target() == TObject) {
        // Redo object transformation
        currentObject()->redo();
        if (currentObject()->getRelation() != -1)
            glEngine->getRelatedLight(currentObject())->setPosition3(currentObject()->getPosition3());
    } else if (target() == TLight) {
        // Redo light transformation
        currentLight()->redo();
        if (currentLight()->getRelation() != -1)
            glEngine->removeRelation(currentLight()->getRelation());
    }
    updateStatus();
    statusBar()->showMessage(">> Aldaketa berregin da", 2000);
}

void MainWindow::targetChange(int index) {
    rightPanel->getCheckRefSystem()->setEnabled(true);
    rightPanel->getBtnAllPos()->setEnabled(false);
    rightPanel->getBtnAllNeg()->setEnabled(false);
    // Update menu
    if (index == TObject) {
        if (operation() == Scale) {
            // Enable buttons to apply on the 3 dimensions
            rightPanel->getBtnAllPos()->setEnabled(true);
            rightPanel->getBtnAllNeg()->setEnabled(true);
        }
        actTargetObject->setChecked(true);
        statusBar()->showMessage(">> Aukeratutako objektuari aplikatzen", 2000);
    } else if (index == TCamera) {
        rightPanel->getCheckRefSystem()->setChecked(true);
        rightPanel->getCheckRefSystem()->setEnabled(false);
        actTargetCamera->setChecked(true);
        statusBar()->showMessage(">> Kamerari aplikatzen", 2000);
    } else {
        actTargetLight->setChecked(true);
        statusBar()->showMessage(">> Argiari aplikatzen", 2000);
    }
    updateStatus();
}

void MainWindow::targetChange(QAction* action) {
    rightPanel->getCheckRefSystem()->setEnabled(true);
    rightPanel->getBtnAllPos()->setEnabled(false);
    rightPanel->getBtnAllNeg()->setEnabled(false);
    // Update combo box
    if (action->text() == "Objektua") {
        if (operation() == Scale) {
            // Enable buttons to apply on the 3 dimensions
            rightPanel->getBtnAllPos()->setEnabled(true);
            rightPanel->getBtnAllNeg()->setEnabled(true);
        }
        rightPanel->getComboTarget()->setCurrentIndex(TObject);
        statusBar()->showMessage(">> Aukeratutako objektuari aplikatzen", 2000);
    } else if (action->text() == "Kamera") {
        rightPanel->getCheckRefSystem()->setChecked(true);
        rightPanel->getCheckRefSystem()->setEnabled(false);
        rightPanel->getComboTarget()->setCurrentIndex(TCamera);
        statusBar()->showMessage(">> Kamerari aplikatzen", 2000);
    } else {
        rightPanel->getComboTarget()->setCurrentIndex(TLight);
        statusBar()->showMessage(">> Argiari aplikatzen", 2000);
    }
    updateStatus();
}

void MainWindow::operationChange(int index) {
    rightPanel->getBtnAllPos()->setEnabled(false);
    rightPanel->getBtnAllNeg()->setEnabled(false);
    // Show the corresponding spin boxes
    rightPanel->getStackedWidget()->setCurrentIndex(index);
    // Update menu
    if (index == Move) {
        actOperationMove->setChecked(true);
        statusBar()->showMessage(">> Aldaketa: Mugitu", 2000);
    } else if (index == Scale) {
        if (target() == TObject) {
            // Enable buttons to apply on the 3 dimensions
            rightPanel->getBtnAllPos()->setEnabled(true);
            rightPanel->getBtnAllNeg()->setEnabled(true);
        }
        actOperationScale->setChecked(true);
        statusBar()->showMessage(">> Aldaketa: Tamaina aldatu", 2000);
    } else {
        actOperationRotate->setChecked(true);
        statusBar()->showMessage(">> Aldaketa: Biratu", 2000);
    }
}

void MainWindow::operationChange(QAction* action) {
    rightPanel->getBtnAllPos()->setEnabled(false);
    rightPanel->getBtnAllNeg()->setEnabled(false);
    // Update combo box and show the corresponding spin boxes
    if (action->text() == "Mugitu") {
        rightPanel->getComboOperation()->setCurrentIndex(Move);
        rightPanel->getStackedWidget()->setCurrentIndex(Move);
        statusBar()->showMessage(">> Aldaketa: Mugitu", 2000);
    } else if (action->text() == "Tamaina aldatu") {
        if (target() == TObject) {
            // Enable buttons to apply on the 3 dimensions
            rightPanel->getBtnAllPos()->setEnabled(true);
            rightPanel->getBtnAllNeg()->setEnabled(true);
        }
        rightPanel->getComboOperation()->setCurrentIndex(Scale);
        rightPanel->getStackedWidget()->setCurrentIndex(Scale);
        statusBar()->showMessage(">> Aldaketa: Tamaina aldatu", 2000);
    } else {
        rightPanel->getComboOperation()->setCurrentIndex(Rotate);
        rightPanel->getStackedWidget()->setCurrentIndex(Rotate);
        statusBar()->showMessage(">> Aldaketa: Biratu", 2000);
    }
}

void MainWindow::settings() {
    // Show dialog
    settingsDialog->load(glEngine->getSettings());
    if (settingsDialog->exec() == QDialog::Accepted) {
        // Update camera and lighting toggle menus
        actQuadView->setChecked(settingsDialog->getSettings().quadView);
        actToggleLighting->setChecked(settingsDialog->getSettings().lightsEnabled);
        // Update the program settings
        glEngine->setSettings(settingsDialog->getSettings());
        glEngine->updateGL();
        statusBar()->showMessage(">> Hobespenak eguneratu dira", 2000);
    }
}

void MainWindow::transformPositiveX() {
    if (target() == TObject) {
        /** Objektuari aplikatu **/
        if (operation() == Move) { // Mugitu
            currentObject()->move(rightPanel->getMoveDistanceX()->value(), 0.0, 0.0, rightPanel->getCheckRefSystem()->isChecked());
            if (currentObject()->getRelation() != -1)
                glEngine->getRelatedLight(currentObject())->setPosition3(currentObject()->getPosition3());
        } else if (operation() == Scale) { // Tamaina aldatu
            currentObject()->scale(rightPanel->getScaleFactorX()->value(), 1.0, 1.0, rightPanel->getCheckRefSystem()->isChecked());
            if (currentObject()->getRelation() != -1)
                glEngine->getRelatedLight(currentObject())->setPosition3(currentObject()->getPosition3());
        } else { // Biratu
            currentObject()->rotate(X, rightPanel->getRotateAngleX()->value(), rightPanel->getCheckRefSystem()->isChecked());
            if (currentObject()->getRelation() != -1)
                glEngine->getRelatedLight(currentObject())->setPosition3(currentObject()->getPosition3());
        }
    } else if (target() == TCamera) {
        /** Kamerari aplikatu **/
        if (operation() == Move) { // Mugitu
            if (!glEngine->getCamera()->move(rightPanel->getMoveDistanceX()->value(), 0.0, 0.0))
                statusBar()->showMessage(">> Kamera honi ezin zaio aldaketa hau aplikatu", 2000);
            else if (glEngine->getCamera()->getRelation() != -1)
                glEngine->getRelatedLight(glEngine->getCamera())->followCamera(glEngine->getCamera()->getEye(), glEngine->getCamera()->getAt());
        } else if (operation() == Rotate) { // Biratu
            if (!glEngine->getCamera()->rotate(X, rightPanel->getRotateAngleX()->value()))
                statusBar()->showMessage(">> Kamera honi ezin zaio aldaketa hau aplikatu", 2000);
            else if (glEngine->getCamera()->getRelation() != -1)
                glEngine->getRelatedLight(glEngine->getCamera())->followCamera(glEngine->getCamera()->getEye(), glEngine->getCamera()->getAt());
        } else { // Tamaina aldatu
            statusBar()->showMessage(">> Kamerak ezin dira tamainaz aldatu", 2000);
        }
    } else {
        /** Argiari aplikatu **/
        if (operation() == Move) { // Mugitu
            currentLight()->move(rightPanel->getMoveDistanceX()->value(), 0.0, 0.0, rightPanel->getCheckRefSystem()->isChecked());
            if (currentLight()->getRelation() != -1)
                glEngine->removeRelation(currentLight()->getRelation());
        } else if (operation() == Rotate) { // Biratu
            if (currentLight()->getType() == Spotlight) {
                currentLight()->rotate(X, rightPanel->getRotateAngleX()->value(), rightPanel->getCheckRefSystem()->isChecked());
                if (currentLight()->getRelation() != -1)
                    glEngine->removeRelation(currentLight()->getRelation());
            } else {
                statusBar()->showMessage(">> Argi mota hau ezin da biratu", 2000);
            }
        } else { // Tamaina aldatu
            statusBar()->showMessage(">> Argiak ezin dira tamainaz aldatu", 2000);
        }
    }
    updateStatus();
}
        
void MainWindow::transformNegativeX() {
    if (target() == TObject) {
        /** Objektuari aplikatu **/
        if (operation() == Move) { // Mugitu
            currentObject()->move(-rightPanel->getMoveDistanceX()->value(), 0.0, 0.0, rightPanel->getCheckRefSystem()->isChecked());
            if (currentObject()->getRelation() != -1)
                glEngine->getRelatedLight(currentObject())->setPosition3(currentObject()->getPosition3());
        } else if (operation() == Scale) { // Tamaina aldatu
            currentObject()->scale(1.0 / rightPanel->getScaleFactorX()->value(), 1.0, 1.0, rightPanel->getCheckRefSystem()->isChecked());
            if (currentObject()->getRelation() != -1)
                glEngine->getRelatedLight(currentObject())->setPosition3(currentObject()->getPosition3());
        } else { // Biratu
            currentObject()->rotate(X, -rightPanel->getRotateAngleX()->value(), rightPanel->getCheckRefSystem()->isChecked());
            if (currentObject()->getRelation() != -1)
                glEngine->getRelatedLight(currentObject())->setPosition3(currentObject()->getPosition3());
        }
    } else if (target() == TCamera) {
        // Kamerari aplikatu **/
        if (operation() == Move) { // Mugitu
            if (!glEngine->getCamera()->move(-rightPanel->getMoveDistanceX()->value(), 0.0, 0.0))
                statusBar()->showMessage(">> Kamera honi ezin zaio aldaketa hau aplikatu", 2000);
            else if (glEngine->getCamera()->getRelation() != -1)
                glEngine->getRelatedLight(glEngine->getCamera())->followCamera(glEngine->getCamera()->getEye(), glEngine->getCamera()->getAt());
        } else if (operation() == Rotate) { // Biratu
            if (!glEngine->getCamera()->rotate(X, -rightPanel->getRotateAngleX()->value()))
                statusBar()->showMessage(">> Kamera honi ezin zaio aldaketa hau aplikatu", 2000);
            else if (glEngine->getCamera()->getRelation() != -1)
                glEngine->getRelatedLight(glEngine->getCamera())->followCamera(glEngine->getCamera()->getEye(), glEngine->getCamera()->getAt());
        } else { // Tamaina aldatu
            statusBar()->showMessage(">> Kamerak ezin dira tamainaz aldatu", 2000);
        }
    } else {
        /** Argiari aplikatu **/
        if (operation() == Move) { // Mugitu
            currentLight()->move(-rightPanel->getMoveDistanceX()->value(), 0.0, 0.0, rightPanel->getCheckRefSystem()->isChecked());
            if (currentLight()->getRelation() != -1)
                glEngine->removeRelation(currentLight()->getRelation());
        } else if (operation() == Rotate) { // Biratu
            if (currentLight()->getType() == Spotlight) {
                currentLight()->rotate(X, -rightPanel->getRotateAngleX()->value(), rightPanel->getCheckRefSystem()->isChecked());
                if (currentLight()->getRelation() != -1)
                    glEngine->removeRelation(currentLight()->getRelation());
            } else {
                statusBar()->showMessage(">> Argi mota hau ezin da biratu", 2000);
            }
        } else { // Tamaina aldatu
            statusBar()->showMessage(">> Argiak ezin dira tamainaz aldatu", 2000);
        }
    }
    updateStatus();
}

void MainWindow::transformPositiveY() {
    if (target() == TObject) {
        /** Objektuari aplikatu **/
        if (operation() == Move) { // Mugitu
            currentObject()->move(0.0, rightPanel->getMoveDistanceY()->value(), 0.0, rightPanel->getCheckRefSystem()->isChecked());
            if (currentObject()->getRelation() != -1)
                glEngine->getRelatedLight(currentObject())->setPosition3(currentObject()->getPosition3());
        } else if (operation() == Scale) { // Tamaina aldatu
            currentObject()->scale(1.0, rightPanel->getScaleFactorY()->value(), 1.0, rightPanel->getCheckRefSystem()->isChecked());
            if (currentObject()->getRelation() != -1)
                glEngine->getRelatedLight(currentObject())->setPosition3(currentObject()->getPosition3());
        } else { // Biratu
            currentObject()->rotate(Y, rightPanel->getRotateAngleY()->value(), rightPanel->getCheckRefSystem()->isChecked());
            if (currentObject()->getRelation() != -1)
                glEngine->getRelatedLight(currentObject())->setPosition3(currentObject()->getPosition3());
        }
    } else if (target() == TCamera) {
        /** Kamerari aplikatu **/
        if (operation() == Move) { // Mugitu
            if (!glEngine->getCamera()->move(0.0, rightPanel->getMoveDistanceY()->value(), 0.0))
                statusBar()->showMessage(">> Kamera honi ezin zaio aldaketa hau aplikatu", 2000);
            else if (glEngine->getCamera()->getRelation() != -1)
                glEngine->getRelatedLight(glEngine->getCamera())->followCamera(glEngine->getCamera()->getEye(), glEngine->getCamera()->getAt());
        } else if (operation() == Rotate) { // Biratu
            if (!glEngine->getCamera()->rotate(Y, rightPanel->getRotateAngleY()->value()))
                statusBar()->showMessage(">> Kamera honi ezin zaio aldaketa hau aplikatu", 2000);
            else if (glEngine->getCamera()->getRelation() != -1)
                glEngine->getRelatedLight(glEngine->getCamera())->followCamera(glEngine->getCamera()->getEye(), glEngine->getCamera()->getAt());
        } else { // Tamaina aldatu
            statusBar()->showMessage(">> Kamerak ezin dira tamainaz aldatu", 2000);
        }
    } else {
        /** Argiari aplikatu **/
        if (operation() == Move) { // Mugitu
            currentLight()->move(0.0, rightPanel->getMoveDistanceY()->value(), 0.0, rightPanel->getCheckRefSystem()->isChecked());
            if (currentLight()->getRelation() != -1)
                glEngine->removeRelation(currentLight()->getRelation());
        } else if (operation() == Rotate) { // Biratu
            if (currentLight()->getType() == Spotlight) {
                currentLight()->rotate(Y, rightPanel->getRotateAngleY()->value(), rightPanel->getCheckRefSystem()->isChecked());
                if (currentLight()->getRelation() != -1)
                    glEngine->removeRelation(currentLight()->getRelation());
            } else {
                statusBar()->showMessage(">> Argi mota hau ezin da biratu", 2000);
            }
        } else { // Tamaina aldatu
            statusBar()->showMessage(">> Argiak ezin dira tamainaz aldatu", 2000);
        }
    }
    updateStatus();
}

void MainWindow::transformNegativeY() {
    if (target() == TObject) {
        /** Objektuari aplikatu **/
        if (operation() == Move) { // Mugitu
            currentObject()->move(0.0, -rightPanel->getMoveDistanceY()->value(), 0.0, rightPanel->getCheckRefSystem()->isChecked());
            if (currentObject()->getRelation() != -1)
                glEngine->getRelatedLight(currentObject())->setPosition3(currentObject()->getPosition3());
        } else if (operation() == Scale) { // Tamaina aldatu
            currentObject()->scale(1.0, 1.0 / rightPanel->getScaleFactorY()->value(), 1.0, rightPanel->getCheckRefSystem()->isChecked());
            if (currentObject()->getRelation() != -1)
                glEngine->getRelatedLight(currentObject())->setPosition3(currentObject()->getPosition3());
        } else { // Biratu
            currentObject()->rotate(Y, -rightPanel->getRotateAngleY()->value(), rightPanel->getCheckRefSystem()->isChecked());
            if (currentObject()->getRelation() != -1)
                glEngine->getRelatedLight(currentObject())->setPosition3(currentObject()->getPosition3());
        }
    } else if (target() == TCamera) {
        /** Kamerari aplikatu **/
        if (operation() == Move) { // Mugitu
            if (!glEngine->getCamera()->move(0.0, -rightPanel->getMoveDistanceY()->value(), 0.0)) {
                statusBar()->showMessage(">> Kamera honi ezin zaio aldaketa hau aplikatu", 2000);
            } else if (glEngine->getCamera()->getRelation() != -1) {
                glEngine->getRelatedLight(glEngine->getCamera())->followCamera(glEngine->getCamera()->getEye(), glEngine->getCamera()->getAt());
            }
        } else if (operation() == Rotate) { // Biratu
            if (!glEngine->getCamera()->rotate(Y, -rightPanel->getRotateAngleY()->value()))
                statusBar()->showMessage(">> Kamera honi ezin zaio aldaketa hau aplikatu", 2000);
            else if (glEngine->getCamera()->getRelation() != -1)
                glEngine->getRelatedLight(glEngine->getCamera())->followCamera(glEngine->getCamera()->getEye(), glEngine->getCamera()->getAt());
        } else { // Tamaina aldatu
            statusBar()->showMessage(">> Kamerak ezin dira tamainaz aldatu", 2000);
        }
    } else {
        /** Argiari aplikatu **/
        if (operation() == Move) { // Mugitu
            currentLight()->move(0.0, -rightPanel->getMoveDistanceY()->value(), 0.0, rightPanel->getCheckRefSystem()->isChecked());
            if (currentLight()->getRelation() != -1)
                glEngine->removeRelation(currentLight()->getRelation());
        } else if (operation() == Rotate) { // Biratu
            if (currentLight()->getType() == Spotlight) {
                currentLight()->rotate(Y, -rightPanel->getRotateAngleY()->value(), rightPanel->getCheckRefSystem()->isChecked());
                if (currentLight()->getRelation() != -1)
                    glEngine->removeRelation(currentLight()->getRelation());
            }  else {
                statusBar()->showMessage(">> Argi mota hau ezin da biratu", 2000);
            }
        } else { // Tamaina aldatu
            statusBar()->showMessage(">> Argiak ezin dira tamainaz aldatu", 2000);
        }
    }
    updateStatus();
}

void MainWindow::transformPositiveZ() {
    if (target() == TObject) {
        /** Objektuari aplikatu **/
        if (operation() == Move) { // Mugitu
            currentObject()->move(0.0, 0.0, rightPanel->getMoveDistanceZ()->value(), rightPanel->getCheckRefSystem()->isChecked());
            if (currentObject()->getRelation() != -1)
                glEngine->getRelatedLight(currentObject())->setPosition3(currentObject()->getPosition3());
        } else if (operation() == Scale) { // Tamaina aldatu
            currentObject()->scale(1.0, 1.0, rightPanel->getScaleFactorZ()->value(), rightPanel->getCheckRefSystem()->isChecked());
            if (currentObject()->getRelation() != -1)
                glEngine->getRelatedLight(currentObject())->setPosition3(currentObject()->getPosition3());
        } else { // Biratu
            currentObject()->rotate(Z, rightPanel->getRotateAngleZ()->value(), rightPanel->getCheckRefSystem()->isChecked());
            if (currentObject()->getRelation() != -1)
                glEngine->getRelatedLight(currentObject())->setPosition3(currentObject()->getPosition3());
        }
    } else if (target() == TCamera) {
        /** Kamerari aplikatu **/
        if (operation() == Move) { // Mugitu
            if (!glEngine->getCamera()->move(0.0, 0.0, rightPanel->getMoveDistanceZ()->value()))
                statusBar()->showMessage(">> Kamera honi ezin zaio aldaketa hau aplikatu", 2000);
            else if (glEngine->getCamera()->getRelation() != -1)
                glEngine->getRelatedLight(glEngine->getCamera())->followCamera(glEngine->getCamera()->getEye(), glEngine->getCamera()->getAt());
        } else if (operation() == Rotate) { // Biratu
            if (!glEngine->getCamera()->rotate(Z, rightPanel->getRotateAngleZ()->value()))
                statusBar()->showMessage(">> Kamera honi ezin zaio aldaketa hau aplikatu", 2000);
            else if (glEngine->getCamera()->getRelation() != -1)
                glEngine->getRelatedLight(glEngine->getCamera())->followCamera(glEngine->getCamera()->getEye(), glEngine->getCamera()->getAt());
        } else { // Tamaina aldatu
            statusBar()->showMessage(">> Kamerak ezin dira tamainaz aldatu", 2000);
        }
    } else {
        /** Argiari aplikatu **/
        if (operation() == Move) { // Mugitu
            currentLight()->move(0.0, 0.0, rightPanel->getMoveDistanceZ()->value(), rightPanel->getCheckRefSystem()->isChecked());
            if (currentLight()->getRelation() != -1)
                glEngine->removeRelation(currentLight()->getRelation());
        } else if (operation() == Rotate) { // Biratu
            if (currentLight()->getType() == Spotlight) {
                currentLight()->rotate(Z, rightPanel->getRotateAngleZ()->value(), rightPanel->getCheckRefSystem()->isChecked());
                if (currentLight()->getRelation() != -1)
                    glEngine->removeRelation(currentLight()->getRelation());
            } else {
                statusBar()->showMessage(">> Argi mota hau ezin da biratu", 2000);
            }
        } else { // Tamaina aldatu
            statusBar()->showMessage(">> Argiak ezin dira tamainaz aldatu", 2000);
        }
    }
    updateStatus();
}

void MainWindow::transformNegativeZ() {
    if (target() == TObject) {
        /** Objektuari aplikatu **/
        if (operation() == Move) { // Mugitu
            currentObject()->move(0.0, 0.0, -rightPanel->getMoveDistanceZ()->value(), rightPanel->getCheckRefSystem()->isChecked());
            if (currentObject()->getRelation() != -1)
                glEngine->getRelatedLight(currentObject())->setPosition3(currentObject()->getPosition3());
        } else if (operation() == Scale) { // Tamaina aldatu
            currentObject()->scale(1.0, 1.0, 1.0 / rightPanel->getScaleFactorZ()->value(), rightPanel->getCheckRefSystem()->isChecked());
            if (currentObject()->getRelation() != -1)
                glEngine->getRelatedLight(currentObject())->setPosition3(currentObject()->getPosition3());
        } else { // Biratu
            currentObject()->rotate(Z, -rightPanel->getRotateAngleZ()->value(), rightPanel->getCheckRefSystem()->isChecked());
            if (currentObject()->getRelation() != -1)
                glEngine->getRelatedLight(currentObject())->setPosition3(currentObject()->getPosition3());
        }
    } else if (target() == TCamera) {
        /** Kamerari aplikatu **/
        if (operation() == Move) { // Mugitu
            if (!glEngine->getCamera()->move(0.0, 0.0, -rightPanel->getMoveDistanceZ()->value()))
                statusBar()->showMessage(">> Kamera honi ezin zaio aldaketa hau aplikatu", 2000);
            else if (glEngine->getCamera()->getRelation() != -1)
                glEngine->getRelatedLight(glEngine->getCamera())->followCamera(glEngine->getCamera()->getEye(), glEngine->getCamera()->getAt());
        } else if (operation() == Rotate) { // Biratu
            if (!glEngine->getCamera()->rotate(Z, -rightPanel->getRotateAngleZ()->value()))
                statusBar()->showMessage(">> Kamera honi ezin zaio aldaketa hau aplikatu", 2000);
            else if (glEngine->getCamera()->getRelation() != -1)
                glEngine->getRelatedLight(glEngine->getCamera())->followCamera(glEngine->getCamera()->getEye(), glEngine->getCamera()->getAt());
        } else { // Tamaina aldatu
            statusBar()->showMessage(">> Kamerak ezin dira tamainaz aldatu", 2000);
        }
    } else {
        /** Argiari aplikatu **/
        if (operation() == Move) { // Mugitu
            currentLight()->move(0.0, 0.0, -rightPanel->getMoveDistanceZ()->value(), rightPanel->getCheckRefSystem()->isChecked());
            if (currentLight()->getRelation() != -1)
                glEngine->removeRelation(currentLight()->getRelation());
        } else if (operation() == Rotate) { // Biratu
            if (currentLight()->getType() == Spotlight) {
                currentLight()->rotate(Z, -rightPanel->getRotateAngleZ()->value(), rightPanel->getCheckRefSystem()->isChecked());
                if (currentLight()->getRelation() != -1)
                    glEngine->removeRelation(currentLight()->getRelation());
            } else {
                statusBar()->showMessage(">> Argi mota hau ezin da biratu", 2000);
            }
        } else { // Tamaina aldatu
            statusBar()->showMessage(">> Argiak ezin dira tamainaz aldatu", 2000);
        }
    }
    updateStatus();
}

void MainWindow::transformPositiveAll() {
    // Objektua hiru ardatzetan batera tamaina aldatu
    if (target() == TObject && operation() == Scale) {
        currentObject()->scale(rightPanel->getScaleFactorX()->value(), rightPanel->getScaleFactorY()->value(), rightPanel->getScaleFactorZ()->value(), rightPanel->getCheckRefSystem()->isChecked());
        if (currentObject()->getRelation() != -1)
            glEngine->getRelatedLight(currentObject())->setPosition3(currentObject()->getPosition3());
        updateStatus();
    }
}

void MainWindow::transformNegativeAll() {
    // Objektua hiru ardatzetan batera tamaina aldatu
    if (target() == TObject && operation() == Scale) {
        currentObject()->scale(1.0 / rightPanel->getScaleFactorX()->value(), 1.0 / rightPanel->getScaleFactorY()->value(), 1.0 / rightPanel->getScaleFactorZ()->value(), rightPanel->getCheckRefSystem()->isChecked());
        if (currentObject()->getRelation() != -1)
            glEngine->getRelatedLight(currentObject())->setPosition3(currentObject()->getPosition3());
        updateStatus();
    }
}

void MainWindow::reset() {
    if (target() == TObject) {
        // Objektuaren transformazioak ezabatu
        currentObject()->reset();
        if (currentObject()->getRelation() != -1)
            glEngine->getRelatedLight(currentObject())->setPosition3(currentObject()->getPosition3());
        statusBar()->showMessage(">> Aukeratutako objektua jatorrizko egoerara itzuli da", 2000);
    } else if (target() == TCamera) {
        // Kameraren transformazioak ezabatu
        glEngine->getCamera()->reset();
        if (glEngine->getCamera()->getRelation() != -1)
            glEngine->getRelatedLight(glEngine->getCamera())->followCamera(glEngine->getCamera()->getEye(), glEngine->getCamera()->getAt());
        cameraZoomChange(10);
        statusBar()->showMessage(">> Aukeratutako kamera jatorrizko egoerara itzuli da", 2000);
    } else {
        // Argiaren transformazioak ezabatu
        currentLight()->reset();
        if (currentLight()->getRelation() != -1)
            glEngine->removeRelation(currentLight()->getRelation());
        statusBar()->showMessage(">> Aukeratutako argia jatorrizko egoerara itzuli da", 2000);
    }
    updateStatus();
}

void MainWindow::updateStatus() {
    // Object position label
    if (currentObject() != NULL) {
        const Array4& obj = currentObject()->getPosition();
        leftPanel->getObjectPosition()->setText(QString("Pos: (%1, %2, %3)").arg(obj.v[0], 0, 'g', 3).arg(obj.v[1], 0, 'g', 3).arg(obj.v[2], 0, 'g', 3));
    } else {
        leftPanel->getObjectPosition()->setText("");
    }
    // Camera position label
    if (leftPanel->getComboCameras()->currentIndex() != Ortho) {
        Array3 cam = glEngine->getCamera()->getEye();
        leftPanel->getCameraPosition()->setText(QString("Pos: (%1, %2, %3)").arg(cam.v[0], 0, 'g', 3).arg(cam.v[1], 0, 'g', 3).arg(cam.v[2], 0, 'g', 3));
    } else {
        leftPanel->getCameraPosition()->setText("");
    }
    // Light position label
    if (currentLight() != NULL) {
        const Array4& light = currentLight()->getPosition();
        leftPanel->getLightPosition()->setText(QString("Pos: (%1, %2, %3)").arg(light.v[0], 0, 'g', 3).arg(light.v[1], 0, 'g', 3).arg(light.v[2], 0, 'g', 3));
    } else {
        leftPanel->getLightPosition()->setText("");
    }
    // Undo and redo
    if (target() == TObject && currentObject() != NULL) {
        actUndo->setEnabled(currentObject()->undoable());
        actRedo->setEnabled(currentObject()->redoable());
    } else if (target() == TLight && currentLight() != NULL) {
        actUndo->setEnabled(currentLight()->undoable());
        actRedo->setEnabled(currentLight()->redoable());
    } else {
        actUndo->setEnabled(false);
        actRedo->setEnabled(false);
    }
    glEngine->updateGL();
}

void MainWindow::cameraChange(int index) {
    // Reset zoom
    glEngine->getCamera()->zoom(1.0);
    leftPanel->getSliderZoom()->setValue(10);
    // Update menu and set the camera
    if (index == Ortho) {
        glEngine->setCamera(Ortho);
        actCameraOrtho->setChecked(true);
        statusBar()->showMessage(">> Kamera ortografikotik ikusten", 2000);
    } else if (index == Helicopter) {
        glEngine->setCamera(Helicopter);
        actCameraHelicopter->setChecked(true);
        statusBar()->showMessage(">> Helikoptero kameratik ikusten", 2000);
    } else if (index == ObjectCam) {
        glEngine->setCamera(ObjectCam);
        actCameraObject->setChecked(true);
        statusBar()->showMessage(">> Kamera-objektutik ikusten", 2000);
    } else {
        glEngine->setCamera(FirstPerson);
        actCameraFirstPerson->setChecked(true);
        statusBar()->showMessage(">> Kamera ibiltaritik ikusten", 2000);
    }
    updateStatus();
}

void MainWindow::cameraChange(QAction* action) {
    // Reset zoom
    glEngine->getCamera()->zoom(1.0);
    leftPanel->getSliderZoom()->setValue(10);
    // Update combo box and set the camera
    if (action->text() == "Ortografikoa") {
        glEngine->setCamera(Ortho);
        leftPanel->getComboCameras()->setCurrentIndex(Ortho);
        statusBar()->showMessage(">> Kamera ortografikotik ikusten", 2000);
    } else if (action->text() == "Helikopteroa") {
        glEngine->setCamera(Helicopter);
        leftPanel->getComboCameras()->setCurrentIndex(Helicopter);
        statusBar()->showMessage(">> Helikoptero kameratik ikusten", 2000);
    } else if (action->text() == "Kamera-objektua") {
        glEngine->setCamera(ObjectCam);
        leftPanel->getComboCameras()->setCurrentIndex(ObjectCam);
        statusBar()->showMessage(">> Kamera-objektutik ikusten", 2000);
    } else {
        glEngine->setCamera(FirstPerson);
        leftPanel->getComboCameras()->setCurrentIndex(FirstPerson);
        statusBar()->showMessage(">> Kamera ibiltaritik ikusten", 2000);
    }
    updateStatus();
}

void MainWindow::nextCamera() {
    // Trigger the corresponding action
    if (actCameraFirstPerson->isChecked())
        actCameraOrtho->trigger();
    else if (actCameraOrtho->isChecked())
        actCameraHelicopter->trigger();
    else if (actCameraHelicopter->isChecked())
        actCameraObject->trigger();
    else
        actCameraFirstPerson->trigger();
}

void MainWindow::cameraZoomChange(int factor) {
    int value = leftPanel->getSliderZoom()->value();
    // Check that the zoom factor stays between the limits
    if ((factor == 1 && value < 15) || (factor == -1 && value > 5)) {
        value += factor;
        leftPanel->getSliderZoom()->setValue(value);
    }
    glEngine->getCamera()->zoom(value / 10.0);
    glEngine->updateGL();
    statusBar()->showMessage(QString(">> Zoom-a aldatzen: %1x").arg(value / 10.0), 2000);
}

void MainWindow::cameraLookAtObject() {
    if (leftPanel->getComboCameras()->currentIndex() != Ortho) {
        glEngine->getCamera()->lookAt(currentObject()->getPosition3());
        glEngine->updateGL();
        statusBar()->showMessage(">> Kamera objektura begiratzen jarri da", 2000);
    } else {
        statusBar()->showMessage(">> Operazio hau ezin da burutu kamera orografikoarekin", 2000);
    }
}

void MainWindow::addSun() {
    if (leftPanel->getListLights()->count() < MAX_LIGHTS) {
        // Look for a free position in the list
        int index = 0;
        while (index < leftPanel->getListLights()->count() && ((Light*)leftPanel->getListLights()->item(index))->getId() == index)
            index++;
        // Insert the object into that position and mark it as selected
        leftPanel->getListLights()->addItem(new Light(Sun, index));
        leftPanel->getListLights()->setCurrentRow(index);
        glEngine->updateGL();
        statusBar()->showMessage(">> Eguzki bat gehitu da", 2000);
    } else {
        statusBar()->showMessage(">> Gehienez 8 argi egon daitezke", 2000);
    }
}

void MainWindow::addLightBulb() {
    if (leftPanel->getListLights()->count() < MAX_LIGHTS) {
        // Look for a free position in the list
        int index = 0;
        while (index < leftPanel->getListLights()->count() && ((Light*)leftPanel->getListLights()->item(index))->getId() == index)
            index++;
        // Insert the object into that position and mark it as selected
        leftPanel->getListLights()->addItem(new Light(Lightbulb, index));
        leftPanel->getListLights()->setCurrentRow(index);
        glEngine->updateGL();
        statusBar()->showMessage(">> Bonbilla bat gehitu da", 2000);
    } else {
        statusBar()->showMessage(">> Gehienez 8 argi egon daitezke", 2000);
    }
}

void MainWindow::addSpotlight() {
    if (leftPanel->getListLights()->count() < MAX_LIGHTS) {
        // Look for a free position in the list
        int index = 0;
        while (index < leftPanel->getListLights()->count() && ((Light*)leftPanel->getListLights()->item(index))->getId() == index)
            index++;
        // Insert the object into that position and mark it as selected
        leftPanel->getListLights()->addItem(new Light(Spotlight, index));
        leftPanel->getListLights()->setCurrentRow(index);
        glEngine->updateGL();
        statusBar()->showMessage(">> Foku bat gehitu da", 2000);
    } else {
        statusBar()->showMessage(">> Gehienez 8 argi egon daitezke", 2000);
    }
}

void MainWindow::loadLight() {
    QStringList files = QFileDialog::getOpenFileNames(this, "Select one or more files to open", QString(), "Light description files (*.lgt);;All files (*.*)");
    // For each file, load the light and add it to the list (if the limit of 8 is not reached)
    int loaded = 0;
    for (QStringList::const_iterator it = files.constBegin(); it != files.constEnd() && leftPanel->getListLights()->count() < MAX_LIGHTS; ++it) {
        // Look for a free position in the list
        int index = 0;
        while (index < leftPanel->getListLights()->count() && ((Light*)leftPanel->getListLights()->item(index))->getId() == index)
            index++;
        // Add the new light to the list in that position and mark it as selected
        leftPanel->getListLights()->addItem(new Light(it->toUtf8().constData(), index));
        leftPanel->getListLights()->setCurrentRow(index);
        loaded++;
    }
    glEngine->updateGL();
    // Show message in the status bar
    if (loaded == 0)
        statusBar()->showMessage(">> Ez da argi baliodunik aurkitu", 2000);
    else if (loaded == 1)
        statusBar()->showMessage(">> Argi bat kargatu da", 2000);
    else
        statusBar()->showMessage(QString(">> %1 argi kargatu dira").arg(loaded), 2000);
}

void MainWindow::removeLight() {
    if (leftPanel->getListLights()->count() > 0) {
        // Switch it off
        if (currentLight()->isOn())
            currentLight()->toggle();
        // Remove it from the list
        delete leftPanel->getListLights()->takeItem(leftPanel->getListLights()->currentRow());
        glEngine->updateGL();
        statusBar()->showMessage(">> Argia ezabatu da", 2000);
    } else {
        statusBar()->showMessage(">> Ez dago argirik", 2000);
    }
}

void MainWindow::toggleLight() {
    if (leftPanel->getListLights()->count() > 0) {
        // Toggle light status
        if (currentLight()->toggle())
            statusBar()->showMessage(">> Argia piztu da", 2000);
        else
            statusBar()->showMessage(">> Argia itzali da", 2000);
        glEngine->updateGL();
    } else {
        statusBar()->showMessage(">> Ez dago argirik", 2000);
    }
}

void MainWindow::changeLightType() {
    if (leftPanel->getListLights()->count() > 0) {
        // Change to the next light type
        currentLight()->changeType();
        glEngine->updateGL();
        statusBar()->showMessage(">> Argi mota aldatu da", 2000);
    } else {
        statusBar()->showMessage(">> Ez dago argirik", 2000);
    }
}

void MainWindow::assignLightToObject() {
    if (currentObject()->getRelation() != -1) {
        statusBar()->showMessage(">> Objektuak badu argi bat esleituta", 2000);
    } else if (currentLight()->getRelation() != -1) {
        statusBar()->showMessage(">> Argia badago esleituta kamera edo objektu batera", 2000);
    } else {
        // Set the relationship
        glEngine->createRelation(currentLight(), currentObject());
        updateStatus();
        statusBar()->showMessage(">> Argia objektuari esleitu zaio", 2000);
    }
}

void MainWindow::assignLightToCamera() {
    if (glEngine->getCamera()->getRelation() != -1) {
        statusBar()->showMessage(">> Kamerak badu argi bat esleituta", 2000);
    } else if (actCameraOrtho->isChecked()) {
        statusBar()->showMessage(">> Kamera ortografikoari ezin zaio argi bat esleitu", 2000);
    } else if (currentLight()->getRelation() != -1) {
        statusBar()->showMessage(">> Argia badago esleituta kamera edo objektu batera", 2000);
    } else {
        // Set the relationship
        glEngine->createRelation(currentLight());
        updateStatus();
        statusBar()->showMessage(">> Argia kamerari esleitu zaio", 2000);
    }
}

void MainWindow::unassignLight() {
    if (currentLight()->getRelation() == -1) {
        statusBar()->showMessage(">> Argia ez dago esleituta kamera edo objektu batera", 2000);
    } else {
        // Unset the relationship
        glEngine->removeRelation(currentLight()->getRelation());
        updateStatus();
        statusBar()->showMessage(">> Argiaren esleipena kendu da", 2000);
    }
}

void MainWindow::showHelp() {
    QUrl file = QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/erabiltzaile-gida.pdf");
    if (!QDesktopServices::openUrl(file))
        statusBar()->showMessage(">> Ezin da erabiltzaile gida ireki", 2000);
}