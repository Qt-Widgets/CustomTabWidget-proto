#include "customtabwidget.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStyleFactory>
#include <QGuiApplication>
#include <customdockwidget.h>
#include <QDebug>
#include <assert.h>
#include <QResource>
#include <QFile>

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

    mStyleSheet = loadFile(":/style/stylesheet.css");
    setStyleSheet("QMainWindow { background-color: #08080d; }");

    //temp widgets
    Splitter* splitter = new Splitter(Qt::Horizontal);
    splitter->addWidget(new TabWidgetContainer(this));
    mSplitters.append(splitter);
    setCentralWidget(splitter);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::splitTabWidget(
          int sourceTabIndex
        , TabWidgetContainer* sourceContainer
        , TabWidgetContainer* targetContainer
        , utils::DropArea dropArea)
{
    //find splitter for the target widget.
    int targetSplitterIndex;
    int sourceSplitterIndex;
    TabWidgetContainer& r_targetContainer = *targetContainer;
    TabWidgetContainer& r_sourceContainer = *sourceContainer;
    Splitter* targetSplitter = findSplitter(r_targetContainer, targetSplitterIndex);
    Splitter* sourceSplitter = findSplitter(r_sourceContainer, sourceSplitterIndex);
    QList<int> targetSizes = targetSplitter->sizes();

    if (!targetSplitter) {
        qDebug() << "ERROR: Could not find splitter for the widget.";
        return;
    }

    bool vertical = (targetSplitter->orientation() == Qt::Vertical);
    bool dropVertically = (dropArea == utils::BOTTOM || dropArea == utils::TOP);
    bool dropToRight = (dropArea == utils::BOTTOM || dropArea == utils::RIGHT);

    if (vertical == dropVertically) {
        // insert to splitter if splitter.orientation and dropArea direction are aligned
        // notice that this also means if vertical and dropVertically are both false.
        targetSplitterIndex = dropToRight ? targetSplitterIndex + 1 : targetSplitterIndex;

        if (sourceContainer->hasOnlyOneTab()) {
            targetSplitter->insertWidget(targetSplitterIndex, sourceContainer);
        } else {
            targetSplitter->insertWidget(targetSplitterIndex,
                                       new TabWidgetContainer(this, sourceContainer->tab(sourceTabIndex)));
        }
    } else {
        // Split Widget.
        if (sourceContainer->hasOnlyOneTab() && targetSplitter == sourceSplitter && targetSplitterIndex == sourceSplitterIndex) {
            // Cancel operation if trying to split self when there is only one tab.
            return;
        }

        // create new splitter in opposite direction of the targetSplitter.
        Qt::Orientation splitDirection = vertical ? Qt::Horizontal : Qt::Vertical;

        // add new splitter to target location
        Splitter* newSplitter = new Splitter();
        newSplitter->setOrientation(splitDirection);
        targetSplitter->insertWidget(targetSplitterIndex, newSplitter);

        int dropIndex = dropToRight ? targetSplitterIndex + 1 : targetSplitterIndex;

        // add target targetContainer and newContainer under the new splitter.
        newSplitter->addWidget(targetContainer);
        if (sourceContainer->hasOnlyOneTab()) {
            newSplitter->insertWidget(dropIndex, sourceContainer);
        } else {
            newSplitter->insertWidget(dropIndex, new TabWidgetContainer(MainWindow::instance(),
                                                                      sourceContainer->tab(sourceTabIndex)));
        }

        // restore handle positions
        int splitterPos = vertical ? targetContainer->size().height() / 2.0 : targetContainer->size().width() / 2.0;
        newSplitter->setSizes({splitterPos, splitterPos});
        targetSplitter->setSizes(targetSizes);

        // new splitter is added to a vector so that they can all be easily managed and cleaned.
        mSplitters.append(newSplitter);
    }
}

Splitter *MainWindow::findSplitter(TabWidgetContainer& target, int& index) {
    for (Splitter* layout : mSplitters) {
        TabWidgetContainer* target_ptr = &target;
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
    QVector<Splitter*> splittersToRemove;
    for (Splitter* splitter : mSplitters) {
        if (!splitter || splitter->count() == 0) {
            splittersToRemove.append(splitter);
        }
    }
    for (Splitter* splitter : splittersToRemove) {
        mSplitters.removeOne(splitter);
        if (splitter) {
            splitter->deleteLater();
        }
    }
}

void MainWindow::registerContainer(TabWidgetContainer *container) {
    connect(container, SIGNAL(emptyContainer(TabWidgetContainer*)), this, SLOT(onEmptyContainer(TabWidgetContainer*)));
}

void MainWindow::unRegisterContainer(TabWidgetContainer *container) {
    disconnect(container, SIGNAL(emptyContainer(TabWidgetContainer*)), this, SLOT(onEmptyContainer(TabWidgetContainer*)));
}

void MainWindow::onEmptyContainer(TabWidgetContainer* container) {
    int index;
    TabWidgetContainer& r_container = *container;
    Splitter* layout = findSplitter(r_container, index);
    //layout->removeWidget(container);

    if (container) {
        removeDockWidget(container);
    }
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


