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

Splitter *SplitterManager::createSplitter() {
    Splitter* splitter = new Splitter();
    mSplitters.append(splitter);
    return splitter;
}

void SplitterManager::deleteSplitter(Splitter *splitter) {
    mSplitters.removeOne(splitter);
    splitter->deleteLater();
}

void SplitterManager::deleteEmptySplitter(Splitter* itemToDelete) {
    if (!itemToDelete) {
        return;
    }

    Splitter* parent = nullptr;
    for (Splitter* splitter : mSplitters) {
        int i = splitter->indexOf(static_cast<QWidget*>(itemToDelete));
        if (i > 0) {
            //container's splitter found
            parent = splitter;
            break;
        }
    }

    if (!parent) {
        return;
    }

    bool hasContainers = itemToDelete->hasContainers();
    if (hasContainers) {
        //do not delete if itemToDelete has containers.
        return;
    }

    QList<Splitter*> splitters = itemToDelete->getSplitters();
    if (splitters.count() > 1) {
        //do not delete if itemToDelete has more than one splitters
        return;
    } else if (splitters.count() == 1) {
        //move child splitter directly under the parent splitter.
        parent->addWidget(splitters.first());
    }

    itemToDelete->deleteLater();
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
        //count <= 1 because this container has not been deleted yet.
        if (parentSplitter->getWidgets().count() <= 1) {
            // parentSplitter is empty, so delete it.
            deleteSplitter(parentSplitter);
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
        Splitter* newSplitter = createSplitter();
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
            //container's splitter found
            index = i;
            return splitter;
        }
    }
    return nullptr;
}
