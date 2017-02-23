#include "customdockwidget.h"
#include <customtabwidget.h>
#include <QTabBar>
#include <mainwindow.h>
#include <QDebug>

TabWidgetContainer::TabWidgetContainer(QWidget *parent, QWidget* tab)
    : QDockWidget(parent)
{
    setFeatures(NoDockWidgetFeatures);
    setTitleBarWidget(new QWidget(this));

    //temp add some dummy tabs
    if (!tab) {
        tab = new QWidget(this);
    }
    int runningNumber = MainWindow::instance()->getRunningNumber();
    mTabWidget.addTab(tab, QString("tab").append(QString::number(runningNumber)));

    setWidget(&mTabWidget);

    connect(&mTabWidget, SIGNAL(checkIfEmptyContainer()), this, SLOT(onCheckIfEmptyContainer()));
    MainWindow::instance()->registerContainer(this);
}

TabWidgetContainer::~TabWidgetContainer() {
    disconnect(&mTabWidget, SIGNAL(checkIfEmptyContainer()), this, SLOT(onCheckIfEmptyContainer()));
    MainWindow::instance()->unRegisterContainer(this);
}

bool TabWidgetContainer::hasOnlyOneTab() {
    return mTabWidget.tabBar()->count() == 1;
}

bool TabWidgetContainer::hasNoTabs() {
    return mTabWidget.tabBar()->count() == 0;
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

void TabWidgetContainer::onCheckIfEmptyContainer() {
    if (hasNoTabs()) {
        qDebug() << "has no tabs";
        emit emptyContainer(this);
    }
}
