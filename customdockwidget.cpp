#include "customdockwidget.h"
#include <customtabwidget.h>

customDockWidget::customDockWidget(QWidget *parent)
    : QDockWidget(parent)
    , mTabWidget(new TabWidget(this))
{
    setFeatures(NoDockWidgetFeatures);
    setTitleBarWidget(new QWidget(this));

    //temp add some dummy tabs
    mTabWidget->addTab(new QWidget(this), "tab1");

    setWidget(mTabWidget);
}

QWidget *customDockWidget::tabWidget() {
    return mTabWidget;
}
