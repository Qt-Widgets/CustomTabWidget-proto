#include "customdockwidget.h"
#include <customtabwidget.h>

customDockWidget::customDockWidget(QWidget *parent, QWidget* tab)
    : QDockWidget(parent)
    , mTabWidget(new TabWidget(this))
{
    setFeatures(NoDockWidgetFeatures);
    setTitleBarWidget(new QWidget(this));

    //temp add some dummy tabs
    mTabWidget->addTab(tab, "tab1");

    setWidget(mTabWidget);
    setStyleSheet("*{ background-color: #AAAAAA; }");
}

QWidget *customDockWidget::tabWidget() {
    return mTabWidget;
}
