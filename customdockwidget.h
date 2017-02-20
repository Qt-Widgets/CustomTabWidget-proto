#pragma once

#include <QDockWidget>
#include <QVBoxLayout>
#include <customtabwidget.h>

class customDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit customDockWidget(QWidget *parent = 0, QWidget *tab = 0);
    ~customDockWidget();
    bool hasOnlyOneTab();
    bool hasNoTabs();
    QWidget* tab(int index);
    void insertTab(int index, QWidget* tab, const QString& label);
    void addTab(QWidget* tab, const QString &label);

public slots:
    void onCheckIfEmptyContainer();

signals:
    void emptyContainer(customDockWidget*);

private:
    TabWidget mTabWidget;
};

