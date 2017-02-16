#pragma once
#include <QTabWidget>
#include <QFrame>
#include <QPushButton>

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
    void paintEvent(QPaintEvent* event) override;

private slots:
    void addTabTest();
    void on_tabBarClicked(int index);

private:
    void drawIndicator();

private:
    QPushButton* mMenuButton;
    QRect mDragIndicator = QRect();
    const int mIndicatorMargin = 10;
    enum Area {
        TOP, RIGHT, BOTTOM, LEFT, INVALID
    };
    Area mIndicatorArea = Area::INVALID;
};
