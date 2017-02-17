#pragma once
#include <QTabWidget>
#include <QFrame>
#include <QPushButton>

#include <drawoverlay.h>

class TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget *parent = 0);

protected:
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void addTabTest();
    void on_tabBarClicked(int index);

private:
    void updateIndicatorRect();

private:
    QPushButton* mMenuButton;
    const int mIndicatorMargin = 10;
    enum Area {
        TABBAR, TOP, RIGHT, BOTTOM, LEFT, INVALID
    };
    Area mIndicatorArea = Area::INVALID;
    DrawOverlay* mDrawOverlay;
};
