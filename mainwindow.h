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

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void splitTabWidget(QWidget* source, QWidget* target, utils::DropArea dropArea);
    QBoxLayout* findWidgetLayout(QWidget* target, QBoxLayout* targetLayout);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
