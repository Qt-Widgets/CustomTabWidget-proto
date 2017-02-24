#include <QTabBar>
#include <mainwindow.h>
#include <QDebug>

#include <include/dockingSystem/tabwidget.h>
#include <include/dockingSystem/dockwidget.h>

TabWidgetContainer::TabWidgetContainer(QWidget *parent, QWidget* tab, QString title)
    : QDockWidget(parent)
{
    setFeatures(NoDockWidgetFeatures);
    setTitleBarWidget(new QWidget(this));

    if (!tab) {
        //temp add some dummy tabs
        tab = new QWidget(this);
    }

    if (title == "") {
        //temp add some dummy title
        title = QString("tab").append(QString::number(MainWindow::instance()->getRunningNumber()));
    }

    mTabWidget.addTab(tab, title);
    setWidget(&mTabWidget);

    //testIfEmpty signal is just passing trough.
    connect(&mTabWidget, SIGNAL(testIfEmpty()), this, SIGNAL(testIfEmpty()));

    MainWindow::instance()->registerContainer(this);
}

bool TabWidgetContainer::hasOneTab() {
    return mTabWidget.tabBar()->count() == 1;
}

bool TabWidgetContainer::hasTabs() {
    return mTabWidget.tabBar()->count() > 0;
}

QWidget *TabWidgetContainer::tab(int index) {
    return mTabWidget.widget(index);
}

void TabWidgetContainer::insertTab(int index, QWidget *tab, const QString& label) {
    mTabWidget.insertTab(index, tab, label);
}

void TabWidgetContainer::addTab(QWidget *tab, const QString& label) {
    mTabWidget.addTab(tab, label);
}
