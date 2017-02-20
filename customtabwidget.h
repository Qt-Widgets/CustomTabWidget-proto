#pragma once
#include <QTabWidget>
#include <QFrame>
#include <QPushButton>
#include <utils.h>

#include <drawoverlay.h>

class TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget *parent = 0);
    ~TabWidget();

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

signals:
    void checkIfEmptyContainer();

private:
    void updateIndicatorRect();
    void updateIndicatorArea(QPoint& p);

private:
    QPushButton* mMenuButton;
    const int mIndicatorMargin = 10;
    utils::DropArea mIndicatorArea = utils::DropArea::INVALID;
    DrawOverlay* mDrawOverlay;
};
