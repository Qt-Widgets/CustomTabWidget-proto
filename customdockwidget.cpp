#include "customdockwidget.h"
#include <customtabwidget.h>
#include <QTabBar>
#include <mainwindow.h>
#include <QDebug>

customDockWidget::customDockWidget(QWidget *parent, QWidget* tab)
    : QDockWidget(parent)
{
    setFeatures(NoDockWidgetFeatures);
    setTitleBarWidget(new QWidget(this));

    //temp add some dummy tabs
    if (!tab) {
        tab = new QWidget(this);
    }
    mTabWidget.addTab(tab, "tab1");

    setWidget(&mTabWidget);
    setStyleSheet("*{ background-color: #AAAAAA; }");
    connect(&mTabWidget, SIGNAL(checkIfEmptyContainer()), this, SLOT(onCheckIfEmptyContainer()));
    MainWindow::instance()->registerContainer(this);
}

customDockWidget::~customDockWidget() {
    disconnect(&mTabWidget, SIGNAL(checkIfEmptyContainer()), this, SLOT(onCheckIfEmptyContainer()));
    MainWindow::instance()->unRegisterContainer(this);
}

bool customDockWidget::hasOnlyOneTab() {
    return mTabWidget.tabBar()->count() == 1;
}

bool customDockWidget::hasNoTabs() {
    return mTabWidget.tabBar()->count() == 0;
}

QWidget *customDockWidget::tab(int index) {
    return mTabWidget.widget(index);
}

void customDockWidget::insertTab(int index, QWidget *tab, const QString& label) {
    mTabWidget.insertTab(index, tab, label);
}

void customDockWidget::addTab(QWidget *tab, const QString& label) {
    mTabWidget.addTab(tab, label);
}

void customDockWidget::onCheckIfEmptyContainer() {
    if (hasNoTabs()) {
        qDebug() << "has no tabs";
        emit emptyContainer(this);
    }
}
