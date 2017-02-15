#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStyleFactory>
#include <QGuiApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QApplication::setEffectEnabled(Qt::UI_AnimateMenu, false);
    QApplication::setEffectEnabled(Qt::UI_FadeMenu, false);
    QApplication::setEffectEnabled(Qt::UI_AnimateCombo, false);
    QApplication::setEffectEnabled(Qt::UI_AnimateTooltip, false);
    QApplication::setEffectEnabled(Qt::UI_FadeTooltip, false);
    QApplication::setEffectEnabled(Qt::UI_AnimateToolBox, false);
    QApplication::setAttribute(Qt::AA_CompressHighFrequencyEvents);
    setAnimated(false);

    ui->setupUi(this);
    QApplication::setStyle(QStyleFactory::create("fusion"));
}

MainWindow::~MainWindow() {
    delete ui;
}
