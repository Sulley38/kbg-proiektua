/* 
 * Konputagailu bidezko Grafikoak
 * 
 * Iván Matellanes eta Adrián Núñez
 * 2013 - 2014 ikasturtea
 */

#include <QApplication>
#include "MainWindow.h"

/*
 * Application entry point
 */
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
