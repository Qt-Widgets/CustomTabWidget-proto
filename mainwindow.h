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
    void registerContainer(customDockWidget* container);
    void unRegisterContainer(customDockWidget* container);
    QString styleSheetFile() { return mStyleSheet; }

public slots:
    void onEmptyContainer(customDockWidget *container);

private:
    Ui::MainWindow *ui;
    QVector<QBoxLayout*> mLayouts;
    QString mStyleSheet;
    QString loadFile(QString fileName);
};

#endif // MAINWINDOW_H
