#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application style
    app.setStyle("Fusion");
    
    // Create and show the main window
    MainWindow mainWindow;
    mainWindow.show();
    
    return app.exec();
} 