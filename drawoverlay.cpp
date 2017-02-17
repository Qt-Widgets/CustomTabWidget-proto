#include "drawoverlay.h"
#include <QPainter>

DrawOverlay::DrawOverlay(QWidget *parent) : QWidget(parent) {

}

void DrawOverlay::setRect(QRect rect) {
    mRect = rect;
}

void DrawOverlay::paintEvent(QPaintEvent *event) {
    if (mRect != QRect()) {
        QPainter painter(this);
        painter.setPen(QPen(QBrush(Qt::blue), 3));
        painter.drawRect(mRect);
    } else {
        QPainter painter(this);
        painter.setPen(QPen(QBrush(Qt::blue), 3 /*Qt::DashLine*/));
        painter.drawRect(QRect(0,0,200,200));
    }
}
