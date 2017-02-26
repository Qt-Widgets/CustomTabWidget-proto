#include <QWidget>
#include <QDebug>

#include <include/dockingSystem/splittermanager.h>
#include <include/dockingSystem/tabwidgetcontainer.h>
#include <include/dockingSystem/splitter.h>

SplitterManager::SplitterManager(QWidget *parent) : QWidget(parent) {
    //temp widgets
    Splitter* splitter = new Splitter(Qt::Horizontal);
    splitter->addWidget(new TabWidgetContainer(this));
    setTopSplitter(splitter);
    mSplitters.append(splitter);
}

void SplitterManager::registerContainer(TabWidgetContainer *container) {
    connect(container, SIGNAL(testIfEmpty()), this, SLOT(onEmptyContainer()));
}

void SplitterManager::onEmptyContainer() {
    TabWidgetContainer* container = static_cast<TabWidgetContainer*>(sender());
    if (!container || container->hasTabs()) {
        return;
    }

    int index = 0;
    TabWidgetContainer& r_container = *container;
    Splitter* parentSplitter = findSplitter(r_container, index);

    // is empty container, so delete it.
    container->deleteLater();

    if (parentSplitter) {
        if (parentSplitter->getWidgets().count() <= 1) {
            // parentSplitter is empty, so delete it.
            mSplitters.removeOne(parentSplitter);
            parentSplitter->deleteLater();
        }
    }
}

void SplitterManager::splitTabWidget(
          int sourceTabIndex
        , TabWidgetContainer* sourceContainer
        , TabWidgetContainer* targetContainer
        , utils::DropArea dropArea
        , QString tabTitle)
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

        if (sourceContainer->hasOneTab()) {
            targetSplitter->insertWidget(targetSplitterIndex, sourceContainer);
        } else {
            targetSplitter->insertWidget(targetSplitterIndex, new TabWidgetContainer(
                                             this,
                                             sourceContainer->tab(sourceTabIndex),
                                             tabTitle));
        }
    } else {
        // Split Widget.
        if (sourceContainer->hasOneTab() && targetSplitter == sourceSplitter && targetSplitterIndex == sourceSplitterIndex) {
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
        if (sourceContainer->hasOneTab()) {
            newSplitter->insertWidget(dropIndex, sourceContainer);
        } else {
            newSplitter->insertWidget(dropIndex, new TabWidgetContainer(this,
                                                                      sourceContainer->tab(sourceTabIndex),
                                                                      tabTitle));
        }

        // restore handle positions
        int splitterPos = vertical ? targetContainer->size().height() / 2.0 : targetContainer->size().width() / 2.0;
        newSplitter->setSizes({splitterPos, splitterPos});
        targetSplitter->setSizes(targetSizes);

        // new splitter is added to a vector so that they can all be easily managed and removed.
        mSplitters.append(newSplitter);
    }
}

Splitter* SplitterManager::findSplitter(TabWidgetContainer& target, int& index) {
    for (Splitter* splitter : mSplitters) {
        TabWidgetContainer* target_ptr = &target;
        int i = splitter->indexOf(static_cast<QWidget*>(target_ptr));
        if (i < 0) {
            //target not found in this splitter
            continue;
        } else {
            index = i;
            return splitter;
        }
    }
    return nullptr;
}
