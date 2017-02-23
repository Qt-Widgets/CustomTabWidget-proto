#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <customdockwidget.h>
#include <utils.h>
#include <splitter.h>

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
                        TabWidgetContainer* sourceContainer,
                        TabWidgetContainer* targetContainer,
                        utils::DropArea dropArea);
    Splitter* findSplitter(TabWidgetContainer &target, int &index);
    void clearEmptyLayouts();
    void registerContainer(TabWidgetContainer* container);
    void unRegisterContainer(TabWidgetContainer* container);
    QString styleSheetFile() { return mStyleSheet; }
    int getRunningNumber() { return ++mRunningCount; }

public slots:
    void onEmptyContainer(TabWidgetContainer *container);

private:
    Ui::MainWindow *ui;
    QVector<Splitter*> mLayouts;
    QString mStyleSheet;
    int mRunningCount = 0;
    QString loadFile(QString fileName);
};

#endif // MAINWINDOW_H
