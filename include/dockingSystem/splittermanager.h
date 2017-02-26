#pragma once
#include <QWidget>
#include <utils.h>

class Splitter;
class TabWidgetContainer;

class SplitterManager : public QWidget
{
    Q_OBJECT

public:
    SplitterManager(QWidget *parent = 0);
    void registerContainer(TabWidgetContainer* container);
    void unRegisterContainer(TabWidgetContainer* container);
    Splitter* createSplitter();
    void deleteSplitter(Splitter* splitter);
    void deleteEmptySplitter(Splitter *itemToDelete);

    void splitTabWidget(int sourceTabIndex,
                        TabWidgetContainer* sourceContainer,
                        TabWidgetContainer* targetContainer,
                        utils::DropArea dropArea,
                        QString tabTitle);
    Splitter* findSplitter(TabWidgetContainer &target, int &index);
    void setTopSplitter(Splitter* widget) { mTopSplitter = widget; }
    Splitter* topSplitter() { return mTopSplitter; }

public slots:
    void onEmptyContainer();

private:
    Splitter* mTopSplitter;
    QVector<Splitter*> mSplitters;
};
