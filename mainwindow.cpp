#include <include/dockingSystem/tabwidget.h>
#include <include/dockingSystem/tabwidgetcontainer.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStyleFactory>
#include <QGuiApplication>
#include <QDebug>
#include <assert.h>
#include <QResource>
#include <QFile>

MainWindow* MainWindow::sInstance = nullptr;
MainWindow* MainWindow::instance() {
    return sInstance;
}

SplitterManager* MainWindow::sSplitterManager = nullptr;
SplitterManager* MainWindow::splitterManager() {
    return sSplitterManager;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    assert(!sInstance); //another instance already exists!
    sInstance = this;

    QApplication::setEffectEnabled(Qt::UI_AnimateMenu, false);
    QApplication::setEffectEnabled(Qt::UI_FadeMenu, false);
    QApplication::setEffectEnabled(Qt::UI_AnimateCombo, false);
    QApplication::setEffectEnabled(Qt::UI_AnimateTooltip, false);
    QApplication::setEffectEnabled(Qt::UI_FadeTooltip, false);
    QApplication::setEffectEnabled(Qt::UI_AnimateToolBox, false);
    QApplication::setAttribute(Qt::AA_CompressHighFrequencyEvents);
    QApplication::setStyle(QStyleFactory::create("fusion"));

    ui->setupUi(this);
    setAnimated(false);

    mStyleSheet = loadFile(":/style/stylesheet.css");
    setStyleSheet("QMainWindow { background-color: #08080d; }");

    assert(!sSplitterManager); //another instance already exists!
    sSplitterManager = new SplitterManager(this);
    setCentralWidget(sSplitterManager->topSplitter());
}

MainWindow::~MainWindow() {
    delete ui;
}

QString MainWindow::loadFile(QString fileName) {
    QResource common(fileName);
    QFile commonFile(common.absoluteFilePath());

    if (!commonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Unable to open file: " << commonFile.fileName() << " besause of error " << commonFile.errorString() << endl;
        return QString();
    }

    QTextStream in(&commonFile);
    QString content = in.readAll();
    return content;
}


