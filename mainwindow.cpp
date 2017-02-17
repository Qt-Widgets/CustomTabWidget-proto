#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStyleFactory>
#include <QGuiApplication>
#include <customdockwidget.h>
#include <QDebug>

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
    QApplication::setStyle(QStyleFactory::create("fusion"));

    ui->setupUi(this);
    setAnimated(false);

    //temp widgets
    ui->horizontalLayout->addWidget(new customDockWidget(this), 1);
    ui->horizontalLayout->addWidget(new customDockWidget(this), 1);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::splitTabWidget(QWidget *source, QWidget *target, utils::DropArea dropArea) {
    //traverse layouts and find layout for the target widget. ui->horizontalLayout is the root of all layouts.
    QBoxLayout* targetLayout = findWidgetLayout(target, ui->horizontalLayout);
    if (!targetLayout) {
        qDebug() << "ERROR: Could not find layout for the widget.";
        return;
    }

    bool vertical = (targetLayout->direction() == QBoxLayout::TopToBottom)
            || (targetLayout->direction() == QBoxLayout::BottomToTop);
    bool dropVertically = (dropArea == utils::BOTTOM || dropArea == utils::TOP);

    if (vertical == dropVertically) {
        qDebug() << "only insert to layout.";
        // insert source to target. source index is targetIndex or targetIndex +1
    } else {
        qDebug() << "split widget.";
        //   1. create a new layout with correct orientation
        //   2. add target widget to the layout
        //   3. add source widget to the layout
    }
}

QBoxLayout* MainWindow::findWidgetLayout(QWidget* target, QBoxLayout* targetLayout) {
    for (int i = 0; i < targetLayout->children().size(); i++) {
        QWidget* foundWidget = static_cast<QWidget*>(targetLayout->children().at(i));
        if (foundWidget && foundWidget == target) {
            return targetLayout;
        }

        //find all layouts recursively
        QBoxLayout* foundLayout = static_cast<QBoxLayout*>(targetLayout->children().at(i));
        if (foundLayout) {
            findWidgetLayout(target, foundLayout);
        }
    }
    return nullptr;
}
