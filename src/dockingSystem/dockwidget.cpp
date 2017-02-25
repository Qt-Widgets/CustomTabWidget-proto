#include <QTabBar>
#include <mainwindow.h>
#include <QDebug>
#include <QMenu>
#include <QAction>

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

    static QString style("QPushButton {"
          "   background-color: #757575;"
          "   padding-left: 4px;"
          "   padding-right: 4px;"
          "   padding-top: 2px;"
          "   padding-bottom: 2px;"
          "}");

    mMenuButton = new QPushButton("+", this);
    mMenuButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    mMenuButton->setStyleSheet(style);
    mTabWidget.setCornerWidget(mMenuButton);

    //testIfEmpty signal is just passing trough.
    connect(&mTabWidget, SIGNAL(testIfEmpty()), this, SIGNAL(testIfEmpty()));

    connect(mMenuButton, SIGNAL(clicked(bool)), this, SLOT(onMenuButtonClicked()));
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

void TabWidgetContainer::onMenuButtonClicked() {
    QMenu contextMenu(tr("Context menu"), this);

    //add tab
    QAction action1("Add Tab", this);
    connect(&action1, SIGNAL(triggered()), this, SLOT(onAction_addTab()));
    contextMenu.addAction(&action1);

    //set floating
    QAction action2("Float", this);
    connect(&action2, SIGNAL(triggered()), this, SLOT(onAction_setFloating()));
    contextMenu.addAction(&action2);

    //create the menu below the button
    QPoint pos = mMenuButton->pos();
    pos.setY(pos.y() + mMenuButton->height());
    contextMenu.exec(mapToGlobal(pos));
}

void TabWidgetContainer::onAction_addTab() {
    int runningNumber = MainWindow::instance()->getRunningNumber();
    addTab(new QFrame(this), QString("tab").append(QString::number(runningNumber)));
}

void TabWidgetContainer::onAction_setFloating() {
    setFloating(true);

    emit testIfEmpty();
    //MainWindow::instance()->clearEmptySplitters();
}
