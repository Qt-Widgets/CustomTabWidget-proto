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
    ui->centralLayout->addWidget(registerContainer(new customDockWidget(this, new QWidget())), 1);
    ui->centralLayout->addWidget(registerContainer(new customDockWidget(this, new QWidget())), 1);
    mLayouts.append(ui->centralLayout);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::splitTabWidget(
          int sourceTabIndex
        , customDockWidget* sourceContainer
        , customDockWidget* targetContainer
        , utils::DropArea dropArea)
{
    //find layout for the target widget. ui->horizontalLayout is the root of all layouts.
    int targetLayoutIndex;
    int sourceLayoutIndex;
    customDockWidget& r_targetContainer = *targetContainer;
    customDockWidget& r_sourceContainer = *sourceContainer;
    QBoxLayout* targetLayout = findWidgetLayout(r_targetContainer, targetLayoutIndex);
    QBoxLayout* sourceLayout = findWidgetLayout(r_sourceContainer, sourceLayoutIndex);
    if (!targetLayout) {
        qDebug() << "ERROR: Could not find layout for the widget.";
        return;
    }

    bool vertical = (targetLayout->direction() == QBoxLayout::TopToBottom)
            || (targetLayout->direction() == QBoxLayout::BottomToTop);
    bool dropVertically = (dropArea == utils::BOTTOM || dropArea == utils::TOP);
    bool dropToRight = (dropArea == utils::BOTTOM || dropArea == utils::RIGHT);

    if (vertical == dropVertically) {
        // inset to layout if layout.direction and dropArea direction are aligned
        targetLayoutIndex = dropToRight ? targetLayoutIndex++ : targetLayoutIndex;

        if (sourceContainer->hasOnlyOneTab()) {
            targetLayout->insertWidget(targetLayoutIndex, sourceContainer, 1);
        } else {
            targetLayout->insertWidget(targetLayoutIndex, new customDockWidget(this, sourceContainer->tab(sourceTabIndex)), 1);
        }
    } else {
        // Split Widget.
        if (sourceContainer->hasOnlyOneTab() && targetLayout == sourceLayout && targetLayoutIndex == sourceLayoutIndex) {
            // Cancel operation if trying to split self when there is only one tab.
            return;
        }

        //QWidget* targetWidget = targetLayout->itemAt(targetLayoutIndex)->widget();
        targetLayout->removeWidget(targetContainer);
        int dropIndex = dropToRight ? 1 : 0;

        // create new layout in opposite direction of the targetLayout.
        QBoxLayout::Direction splitDirection = QBoxLayout::TopToBottom;
        if (vertical) {
            splitDirection = QBoxLayout::LeftToRight;
        }
        QBoxLayout* splitLayout = new QBoxLayout(splitDirection);
        splitLayout->addWidget(targetContainer, 1);
        if (sourceContainer->hasOnlyOneTab()) {
            splitLayout->insertWidget(dropIndex, sourceContainer, 1);
        } else {
            splitLayout->insertWidget(dropIndex, new customDockWidget(this, sourceContainer->tab(sourceTabIndex)), 1);
        }
        targetLayout->insertLayout(targetLayoutIndex, splitLayout, 1);
        mLayouts.append(splitLayout);
    }
}

QBoxLayout *MainWindow::findWidgetLayout(customDockWidget& target, int& index) {
    for (QBoxLayout* layout : mLayouts) {
        customDockWidget* target_ptr = &target;
        int i = layout->indexOf(static_cast<QWidget*>(target_ptr));
        if (i < 0) {
            //target not found in this layout
            continue;
        } else {
            index = i;
            return layout;
        }
    }
    return nullptr;
}

void MainWindow::clearEmptyLayouts() {
    QVector<QBoxLayout*> layoutsToRemove;
    for (QBoxLayout* layout : mLayouts) {
        if (!layout || layout->count() == 0) {
            layoutsToRemove.append(layout);
        }
    }
    for (QBoxLayout* layout : layoutsToRemove) {
        mLayouts.removeOne(layout);
        if (layout) {
            layout->deleteLater();
        }
    }
}

customDockWidget *MainWindow::registerContainer(customDockWidget *container) {
    connect(container, SIGNAL(emptyContainer(customDockWidget*)), this, SLOT(onEmptyContainer(customDockWidget*)));
    return container;
}

void MainWindow::unRegisterContainer(customDockWidget *container) {
    disconnect(container, SIGNAL(emptyContainer(customDockWidget*)), this, SLOT(onEmptyContainer(customDockWidget*)));
}

void MainWindow::onEmptyContainer(customDockWidget* container) {
    int index;
    customDockWidget& r_container = *container;
    QBoxLayout* layout = findWidgetLayout(r_container, index);
    layout->removeWidget(container);
    unRegisterContainer(container);

    // i guess I have to create a post event for the delete?
    container->deleteLater();
}


