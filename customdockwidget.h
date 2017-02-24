#pragma once

#include <QDockWidget>
#include <customtabwidget.h>

class TabWidgetContainer : public QDockWidget
{
    Q_OBJECT

public:
    explicit TabWidgetContainer(QWidget *parent = 0, QWidget *tab = 0, QString title = "");
    virtual ~TabWidgetContainer();
    bool hasOnlyOneTab();
    bool hasNoTabs();
    QWidget* tab(int index);
    void insertTab(int index, QWidget* tab, const QString& label);
    void addTab(QWidget* tab, const QString &label);

public slots:
    void onCheckIfEmptyContainer();

signals:
    void emptyContainer(TabWidgetContainer*);

private:
    TabWidget mTabWidget;
};

