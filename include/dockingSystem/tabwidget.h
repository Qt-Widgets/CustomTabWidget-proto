#pragma once
#include <QTabWidget>
#include <QFrame>
#include <QPushButton>
#include <utils.h>
#include <include/dockingSystem/drawoverlay.h>

class QDrag;

class TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget *parent = 0);
    virtual ~TabWidget();

protected:
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void onMenuButtonClicked();
    void on_tabBarClicked(int index);

signals:
    void testIfEmpty();

private:
    void updateIndicatorRect();
    void updateIndicatorArea(QPoint& p);

private:
    QPushButton* mMenuButton;
    const int mIndicatorMargin = 10;
    utils::DropArea mIndicatorArea = utils::DropArea::INVALID;
    DrawOverlay* mDrawOverlay;
};
