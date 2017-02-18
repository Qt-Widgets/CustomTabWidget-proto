#pragma once

#include <QDockWidget>
#include <QVBoxLayout>

class TabWidget;

class customDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit customDockWidget(QWidget *parent = 0, QWidget *tab = 0);
    QWidget* tabWidget();

private:
    TabWidget* mTabWidget;
};

