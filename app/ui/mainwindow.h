#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QVector>
#include "sidebarbutton.h"
#include "mdiview.h"
#include "coremoduleview.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
    // Get the MDI view
    MdiView* getMdiView() const { return m_mdiView; }

private slots:
    void onSidebarButtonClicked();

private:
    void setupUi();
    void createSidebar();
    void createContentPages();
    
    QWidget *m_centralWidget;
    QHBoxLayout *m_mainLayout;
    
    // Sidebar
    QFrame *m_sidebar;
    QVBoxLayout *m_sidebarLayout;
    QVector<SidebarButton*> m_sidebarButtons;
    
    // Content area
    QStackedWidget *m_contentStack;
    
    // MDI View
    MdiView *m_mdiView;
    
    // Core Module View
    CoreModuleView *m_coreModuleView;
};

#endif // MAINWINDOW_H 