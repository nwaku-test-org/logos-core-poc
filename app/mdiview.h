#ifndef MDIVIEW_H
#define MDIVIEW_H

#include <QWidget>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolBar>

class MdiView : public QWidget
{
    Q_OBJECT

public:
    explicit MdiView(QWidget *parent = nullptr);
    ~MdiView();

private slots:
    void addMdiWindow();
    void toggleViewMode();
    void updateTabCloseButtons();

private:
    void setupUi();
    
    QMdiArea *mdiArea;
    QPushButton *addButton;
    QPushButton *toggleButton;
    QToolBar *toolBar;
    QVBoxLayout *mainLayout;
    
    int windowCounter;
};

#endif // MDIVIEW_H 