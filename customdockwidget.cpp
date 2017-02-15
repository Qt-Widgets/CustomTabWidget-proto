#include "customdockwidget.h"

customDockWidget::customDockWidget(QWidget *parent) : QDockWidget(parent) {
    setFeatures(NoDockWidgetFeatures);
    setTitleBarWidget(new QWidget(this));
}
