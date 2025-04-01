#include "window.h"
#include "ui/mainwindow.h"
#include <QApplication>
#include <QScreen>

Window::Window(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
}

Window::~Window()
{
}

void Window::setupUi()
{
    // Create and set up the main UI content
    MainWindow* mainContent = new MainWindow(this);
    setCentralWidget(mainContent);

    // Set window title and size
    setWindowTitle("Logos Core POC");
    resize(1024, 768);
} 
