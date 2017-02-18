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

    void splitTabWidget(QWidget* source, QWidget* target, utils::DropArea dropArea);
    QBoxLayout* findWidgetLayout(QWidget* target, int &index);

private:
    Ui::MainWindow *ui;
    QVector<QBoxLayout*> mLayouts;
};

#endif // MAINWINDOW_H
