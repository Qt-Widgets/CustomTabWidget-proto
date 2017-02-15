#pragma once

#include <QDockWidget>
#include <QVBoxLayout>
#include <customtabwidget.h>

class customDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit customDockWidget(QWidget *parent = 0);
};

