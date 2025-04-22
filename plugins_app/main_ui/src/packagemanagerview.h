#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QListWidget>
#include <QSplitter>
#include <QTextEdit>
#include <QDir>
#include <QPluginLoader>
#include <QLibrary>
#include <QMap>
#include "core/plugin_registry.h"

class MainWindow;

class PackageManagerView : public QWidget
{
    Q_OBJECT

public:
    explicit PackageManagerView(QWidget *parent = nullptr);
    ~PackageManagerView();
    
    // Set the main window reference
    void setMainWindow(MainWindow* mainWindow) { m_mainWindow = mainWindow; }

private slots:
    void onCategorySelected(int row);
    void onPackageSelected(int row, int column);
    void onReloadClicked();
    void onApplyClicked();

private:
    struct PackageInfo {
        QString name;
        QString installedVersion;
        QString latestVersion;
        QString description;
        QString path;
        QString category;
        QString type;
        bool isLoaded;
    };

    void setupUi();
    void createPackageTable();
    void createCategorySidebar();
    void createPackageDetails();
    void addPackage(const QString& name, const QString& installedVersion, 
                   const QString& latestVersion, const QString& type,
                   const QString& description, bool checked = false);
    void scanPackagesFolder();
    void clearPackageList();
    void addFallbackPackages();
    QList<QString> getSelectedPackages();
    void updateInstallButtonState();
    
    QVBoxLayout *m_layout;
    QSplitter *m_splitter;
    QWidget *m_sidebarWidget;
    QVBoxLayout *m_sidebarLayout;
    QListWidget *m_categoryList;
    QWidget *m_contentWidget;
    QVBoxLayout *m_contentLayout;
    QTableWidget *m_packageTable;
    QHBoxLayout *m_buttonLayout;
    QPushButton *m_reloadButton;
    QPushButton *m_applyButton;
    QTextEdit *m_detailsTextEdit;
    
    QMap<QString, PackageInfo> m_packages;
    
    // Reference to the main window
    MainWindow* m_mainWindow;
}; 