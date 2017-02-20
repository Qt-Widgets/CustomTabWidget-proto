#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <customdockwidget.h>
#include <utils.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    static MainWindow* sInstance;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    static MainWindow* instance();

    void splitTabWidget(int sourceTabIndex,
                        customDockWidget* sourceContainer,
                        customDockWidget* targetContainer,
                        utils::DropArea dropArea);
    QBoxLayout* findWidgetLayout(customDockWidget &target, int &index);
    void clearEmptyLayouts();
    customDockWidget* registerContainer(customDockWidget* container);
    void unRegisterContainer(customDockWidget* container);

public slots:
    void onEmptyContainer(customDockWidget *container);

private:
    Ui::MainWindow *ui;
    QVector<QBoxLayout*> mLayouts;
};

#endif // MAINWINDOW_H
