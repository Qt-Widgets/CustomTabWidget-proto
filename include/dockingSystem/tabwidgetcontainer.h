#pragma once

#include <QDockWidget>
#include <include/dockingSystem/tabwidget.h>

class TabWidgetContainer : public QDockWidget
{
    Q_OBJECT

public:
    explicit TabWidgetContainer(QWidget *parent = 0, QWidget *tab = 0, QString title = "");
    virtual ~TabWidgetContainer() {}
    bool hasOneTab();
    bool hasTabs();

    // providing interface to tabWidget
    QWidget* tab(int index);
    void insertTab(int index, QWidget* tab, const QString& label);
    void addTab(QWidget* tab, const QString &label);
    int getRunningNumber() { return mRunningCount++; }

signals:
    void testIfEmpty();

private slots:
    void onMenuButtonClicked();
    void onAction_addTab();
    void onAction_setFloating();

private:
    TabWidget mTabWidget;
    QPushButton* mMenuButton;
    int mRunningCount = 0;
};

