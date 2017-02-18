#include "customtabwidget.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStyleFactory>
#include <QGuiApplication>
#include <customdockwidget.h>
#include <QDebug>
#include <assert.h>

MainWindow* MainWindow::sInstance = nullptr;

MainWindow* MainWindow::instance() {
    return sInstance;
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

    //temp widgets
    ui->centralLayout->addWidget(new customDockWidget(this, new QWidget(this)), 1);
    ui->centralLayout->addWidget(new customDockWidget(this, new QWidget(this)), 1);
    mLayouts.append(ui->centralLayout);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::splitTabWidget(QWidget *sourceTab, QWidget *target, utils::DropArea dropArea) {
    //find layout for the target widget. ui->horizontalLayout is the root of all layouts.
    int index;
    QBoxLayout* targetLayout = findWidgetLayout(target, index);
    if (!targetLayout) {
        qDebug() << "ERROR: Could not find layout for the widget.";
        return;
    }

    bool vertical = (targetLayout->direction() == QBoxLayout::TopToBottom)
            || (targetLayout->direction() == QBoxLayout::BottomToTop);
    bool dropVertically = (dropArea == utils::BOTTOM || dropArea == utils::TOP);
    bool dropToRight = (dropArea == utils::BOTTOM || dropArea == utils::RIGHT);

    // if layout.direction and dropArea direction are aligned
    if (vertical == dropVertically) {
        index = dropToRight ? index++ : index;
        targetLayout->insertWidget(index, new customDockWidget(this, sourceTab), 1);
    } else {
        // Split Widget.
        QWidget* targetWidget = targetLayout->itemAt(index)->widget();
        targetLayout->removeWidget(targetWidget);
        int dropIndex = dropToRight ? 1 : 0;

        // create new layout in opposite direction of the targetLayout.
        QBoxLayout::Direction splitDirection = QBoxLayout::TopToBottom;
        if (vertical) {
            splitDirection = QBoxLayout::LeftToRight;
        }
        QBoxLayout* splitLayout = new QBoxLayout(splitDirection);
        splitLayout->addWidget(targetWidget, 1);
        splitLayout->insertWidget(dropIndex, new customDockWidget(this, sourceTab), 1);
        targetLayout->insertLayout(index, splitLayout, 1);
        mLayouts.append(splitLayout);
    }
}

QBoxLayout *MainWindow::findWidgetLayout(QWidget* target, int& index) {
    for (QBoxLayout* layout : mLayouts) {
        customDockWidget* foundContainerWidget = static_cast<customDockWidget*>(target->parentWidget());
        if (!foundContainerWidget) {
            continue;
        }

        int i = layout->indexOf(foundContainerWidget);
        if (i < 0) {
            continue;
        }

        QWidget* foundTabWidget = foundContainerWidget->tabWidget();
        if (foundTabWidget == target) {
            index = i;
            return layout;
        }
    }
    return nullptr;
}
