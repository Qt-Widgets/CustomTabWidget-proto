#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <include/dockingSystem/splitter.h>
#include <include/dockingSystem/tabwidgetcontainer.h>
#include <include/dockingSystem/splittermanager.h>
#include <utils.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    static MainWindow* sInstance;
    static SplitterManager* sSplitterManager;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    static MainWindow* instance();
    static SplitterManager* splitterManager();
    QString styleSheetFile() { return mStyleSheet; }

private:
    Ui::MainWindow *ui;
    QString mStyleSheet;
    QString loadFile(QString fileName);
};

#endif // MAINWINDOW_H
