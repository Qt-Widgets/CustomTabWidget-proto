#include "customtabwidget.h"
#include <QTabBar>
#include <QDragMoveEvent>
#include <qdebug.h>
#include <QDrag>
#include <QMimeData>
#include <QWindow>

static QString sourceIndexMimeDataKey() { return QStringLiteral("source/index"); }
static QString sourceTabTitleMimeDataKey() { return QStringLiteral("source/tabtitle"); }

TabWidget::TabWidget(QWidget *parent) : QTabWidget(parent) {
    mDrawOverlay = new DrawOverlay(this);
    mDrawOverlay->raise();

    installEventFilter(this);
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

void TabWidget::dragMoveEvent(QDragMoveEvent* /*event*/) {
    if (mDrawOverlay->getRect() == QRect()) {
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

    bool tabBarArea = (p.y() < tabBar()->rect().height());

    bool topArea = (top + marginY > p.y()) && !tabBarArea;
    bool bottomArea = (height - marginY < p.y()) && !tabBarArea;
    bool rightArea = (marginX < p.x()) && !tabBarArea;
    bool leftArea = (marginX > p.x()) && !tabBarArea;

    if (topArea) {
        mIndicatorArea = Area::TOP;
    } else if (bottomArea) {
        mIndicatorArea = Area::BOTTOM;
    } else if (rightArea) {
        mIndicatorArea = Area::RIGHT;
    } else if (leftArea) {
        mIndicatorArea = Area::LEFT;
    } else if (tabBarArea) {
        mIndicatorArea = Area::TABBAR;
    }
    updateIndicatorRect();
}

void TabWidget::dragEnterEvent(QDragEnterEvent* event) {
    if (event->mimeData()->hasText()) {
        mDrawOverlay->setRect(this->rect());
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void TabWidget::dragLeaveEvent(QDragLeaveEvent* /*event*/) {
    mDrawOverlay->setRect(QRect());
}

void TabWidget::dropEvent(QDropEvent *event) {
    const QMimeData *mime = event->mimeData();
    if (mime->hasText()) {
        int sourceIndex = mime->data(sourceIndexMimeDataKey()).toInt();
        QString tabTitle = QString::fromStdString(mime->data(sourceTabTitleMimeDataKey()).toStdString());
        QTabWidget* sourceTabWidget = static_cast<QTabWidget*>(event->source());
        QWidget* widget = sourceTabWidget->widget(sourceIndex);

        if (mIndicatorArea == Area::TABBAR) {
            QPoint mousePos = tabBar()->mapFromGlobal(QCursor::pos());
            int targetIndex = tabBar()->tabAt(mousePos);
            insertTab(targetIndex, widget, tabTitle);
        } else {
            //todo: split dock widget and create a new custom dockwidget to corresponding area.
        }

        event->acceptProposedAction();
    }

    mDrawOverlay->setRect(QRect());
}

void TabWidget::mousePressEvent(QMouseEvent* event) {
    //click outside of the tab
    QTabWidget::mousePressEvent(event);
    qDebug() << event->pos() << " event->pos()";
}

void TabWidget::mouseReleaseEvent(QMouseEvent */*event*/) {
    //QTabWidget::mouseReleaseEvent(event);
}

void TabWidget::resizeEvent(QResizeEvent* event) {
    QTabWidget::resizeEvent(event);
    if (mDrawOverlay) {
        mDrawOverlay->setGeometry(this->rect());
    }
}

void TabWidget::updateIndicatorRect() {
    if (mIndicatorArea != Area::INVALID) {
        QRect rect = this->rect();
        rect.setTop(tabBar()->rect().height());
        int marginY = rect.height() * 0.4;
        int marginX = rect.width() * 0.4;

        switch (mIndicatorArea) {
        case Area::TOP:
            rect.setBottom(marginY);
            break;
        case Area::BOTTOM:
            rect.setTop(rect.bottom() - marginY);
            break;
        case Area::RIGHT:
            rect.setLeft(rect.right() - marginX);
            break;
        case Area::LEFT:
            rect.setRight(rect.left() + marginX);
            break;
        case Area::TABBAR: {
            QPoint mousePos = tabBar()->mapFromGlobal(QCursor::pos());
            int index = tabBar()->tabAt(mousePos);
            rect = tabBar()->tabRect(index);
            rect.setRight(rect.left() + 5);
            break;
        }
        case Area::INVALID:
            break;
        }

        mDrawOverlay->setRect(rect);
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
    mimeData->setData(sourceTabTitleMimeDataKey(), QByteArray::fromStdString(tabText(index).toStdString()));
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
