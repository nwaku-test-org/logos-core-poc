#include "mainwindow.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Use native system style instead of Fusion
    // app.setStyle("Fusion");
    
    // Set application icon
    app.setWindowIcon(QIcon(":/icons/logos.png"));
    
    // Create and show the main window
    MainWindow mainWindow;
    mainWindow.show();
    
    return app.exec();
} 