#include "customtabwidget.h"
#include <QTabBar>
#include <QDragMoveEvent>
#include <qdebug.h>
#include <QDrag>
#include <QMimeData>
#include <QWindow>
#include <QPainter>

static QString sourceIndexMimeDataKey() { return QStringLiteral("source/index"); }

TabWidget::TabWidget(QWidget *parent) : QTabWidget(parent) {
    setAcceptDrops(true);

    static QString style("QPushButton {"
          "   padding-left: 4px;"
          "   padding-right: 4px;"
          "   padding-top: 2px;"
          "   padding-bottom: 2px;"
          "}");

    mMenuButton = new QPushButton("+", this);
    mMenuButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    mMenuButton->setStyleSheet(style);
    setCornerWidget(mMenuButton);

    connect(mMenuButton, SIGNAL(clicked(bool)), this, SLOT(addTabTest()));
    connect(this, SIGNAL(tabBarClicked(int)), this, SLOT(on_tabBarClicked(int)));
}

void TabWidget::dragMoveEvent(QDragMoveEvent* event) {
    if (mDragIndicator == QRect()) {
        return;
    }

    QPoint p = mapFromGlobal(QCursor::pos());
    if (!rect().contains(p)) {
        //mouse not inside the widget
        return;
    }

    //calculate what area the mouse cursor is in.
    int left = rect().left();
    int top = rect().top();
    int width = rect().width();
    int height = rect().height();
    int marginX = width * 0.5;
    int marginY = height * 0.3;

    bool topArea = (top + marginY > p.y());
    bool bottomArea = (height - marginY < p.y());
    bool rightArea = (marginX < p.x());
    bool leftArea = (marginX > p.x());

    if (topArea) {
        mIndicatorArea = Area::TOP;
    } else if (bottomArea) {
        mIndicatorArea = Area::BOTTOM;
    } else if (rightArea) {
        mIndicatorArea = Area::RIGHT;
    } else if (leftArea) {
        mIndicatorArea = Area::LEFT;
    }
    update();
}

void TabWidget::dragEnterEvent(QDragEnterEvent* event) {
    if (event->mimeData()->hasText()) {
        mDragIndicator = this->rect();
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
    update();
}

void TabWidget::dragLeaveEvent(QDragLeaveEvent*) {
    mDragIndicator = QRect();
    update();
}

void TabWidget::dropEvent(QDropEvent *event) {
    if (event->source() == this) {
        qDebug() << "dropping on it self. There should be a index move operation here.";
        mDragIndicator = QRect();
        update();
        return;
    }

    const QMimeData *mime = event->mimeData();
    if (mime->hasText()) {
        int index = mime->data(sourceIndexMimeDataKey()).toInt();
        QTabWidget* sourceTabWidget = static_cast<QTabWidget*>(event->source());
        QWidget* widget = sourceTabWidget->widget(index);

        this->addTab(widget, widget->objectName() + "_moved");

        event->acceptProposedAction();
    }

    mDragIndicator = QRect();
    update();
}

void TabWidget::mousePressEvent(QMouseEvent* event) {
    //click outside of the tab
    QTabWidget::mousePressEvent(event);
    qDebug() << event->pos() << " event->pos()";
}

void TabWidget::mouseReleaseEvent(QMouseEvent *event) {
    //QTabWidget::mouseReleaseEvent(event);
}

void TabWidget::paintEvent(QPaintEvent *event) {
    QTabWidget::paintEvent(event);

    if (mDragIndicator != QRect()) {
        drawIndicator();
    }
}

void TabWidget::drawIndicator() {
    if (mIndicatorArea != Area::INVALID) {
        QPainter painter(this);
        painter.setPen(QPen(QBrush(Qt::blue), 3, Qt::DashLine));

        QRect rect = mDragIndicator;
        int marginY = rect.height() * 0.4;
        int marginX = rect.width() * 0.4;

        if (mIndicatorArea == Area::TOP) {
            rect.setBottom(marginY);
        } else if (mIndicatorArea == Area::BOTTOM) {
            rect.setTop(rect.bottom() - marginY);
        } else if (mIndicatorArea == Area::RIGHT) {
            rect.setLeft(rect.right() - marginX);
        } else if (mIndicatorArea == Area::LEFT) {
            rect.setRight(rect.left() + marginX);
        }

        painter.drawRect(rect);
    }
}

void TabWidget::addTabTest() {
    addTab(new QFrame(this), "new Tab");
}

void TabWidget::on_tabBarClicked(int index) {
    if (index == -1) {
        return;
    }
    QWidget *tabWidget = widget(index);
    if (!tabWidget) {
        return;
    }

    QMimeData *mimeData = new QMimeData;
    mimeData->setText(tabWidget->objectName());
    mimeData->setData(sourceIndexMimeDataKey(), QByteArray::number(index));

    qreal dpr = window()->windowHandle()->devicePixelRatio();
    QPixmap pixmap(tabBar()->tabRect(index).size() * dpr);
    pixmap.setDevicePixelRatio(dpr);
    tabWidget->render(&pixmap);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);

    if (dropAction == Qt::MoveAction)
        tabWidget->close();
}
