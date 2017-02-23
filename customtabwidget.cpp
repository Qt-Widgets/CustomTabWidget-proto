#include "customtabwidget.h"
#include <QTabBar>
#include <QDragMoveEvent>
#include <qdebug.h>
#include <QDrag>
#include <QMimeData>
#include <QWindow>
#include <mainwindow.h>
#include <assert.h>

static QString sourceIndexMimeDataKey() { return QStringLiteral("source/index"); }
static QString sourceTabTitleMimeDataKey() { return QStringLiteral("source/tabtitle"); }

TabWidget::TabWidget(QWidget *parent)
    : QTabWidget(parent)
    , mDrawOverlay(new DrawOverlay(this))
{
    installEventFilter(this);
    setAcceptDrops(true);
    //setTabsClosable(true);

    static QString style("QPushButton {"
          "   background-color: #757575;"
          "   padding-left: 4px;"
          "   padding-right: 4px;"
          "   padding-top: 2px;"
          "   padding-bottom: 2px;"
          "}");

    mMenuButton = new QPushButton("+", this);
    mMenuButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    mMenuButton->setStyleSheet(style);
    setCornerWidget(mMenuButton);

    connect(mMenuButton, SIGNAL(clicked(bool)), this, SLOT(onMenuButtonClicked()));
    connect(this, SIGNAL(tabBarClicked(int)), this, SLOT(on_tabBarClicked(int)));
}

TabWidget::~TabWidget() {
    disconnect(mMenuButton, SIGNAL(clicked(bool)), this, SLOT(onMenuButtonClicked()));
    disconnect(this, SIGNAL(tabBarClicked(int)), this, SLOT(on_tabBarClicked(int)));
}

void TabWidget::dragMoveEvent(QDragMoveEvent* event) {
    if (mDrawOverlay->getRect() == QRect()) {
        return;
    }

    QPoint p = mapFromGlobal(QCursor::pos());
    if (!rect().contains(p)) {
        //Is this needed?
        //mouse not inside the widget
        return;
    }

    updateIndicatorArea(p);
    updateIndicatorRect();
    event->accept();
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

void TabWidget::dragLeaveEvent(QDragLeaveEvent* event) {
    mDrawOverlay->setRect(QRect());
    event->accept();
}

void TabWidget::dropEvent(QDropEvent *event) {
    const QMimeData *mime = event->mimeData();
    if (mime->hasText()) {
        int sourceIndex = mime->data(sourceIndexMimeDataKey()).toInt();
        QString tabTitle = QString::fromStdString(mime->data(sourceTabTitleMimeDataKey()).toStdString());
        TabWidget* sourceTabWidget = static_cast<TabWidget*>(event->source());
        QWidget* sourceWidget = sourceTabWidget->widget(sourceIndex);
        QPoint mousePos = tabBar()->mapFromGlobal(QCursor::pos());
        int targetIndex = tabBar()->tabAt(mousePos);

        if (mIndicatorArea == utils::DropArea::TABBAR) {
            insertTab(targetIndex, sourceWidget, tabTitle);

            if (sourceTabWidget == this) {
                qDebug() << "change tab order";
            } else {
                qDebug() << "move between two tabWidgets";
            }
        } else {
            TabWidgetContainer* sourceContainer = static_cast<TabWidgetContainer*>(sourceTabWidget->parentWidget());
            TabWidgetContainer* targetContainer = static_cast<TabWidgetContainer*>(this->parentWidget());
            MainWindow::instance()->splitTabWidget(sourceIndex, sourceContainer, targetContainer, mIndicatorArea);
        }

        event->acceptProposedAction();
        event->accept();
        mDrawOverlay->setRect(QRect());
        //emit sourceTabWidget->checkIfEmptyContainer();
        //MainWindow::instance()->clearEmptyLayouts();
    }
}

void TabWidget::mousePressEvent(QMouseEvent* event) {
    //click outside of the tab
    QTabWidget::mousePressEvent(event);
    qDebug() << event->pos() << " event->pos()";
    event->accept();
}

void TabWidget::mouseReleaseEvent(QMouseEvent *event) {
    //QTabWidget::mouseReleaseEvent(event);
    event->accept();
}

void TabWidget::resizeEvent(QResizeEvent* event) {
    QTabWidget::resizeEvent(event);
    if (mDrawOverlay) {
        mDrawOverlay->setGeometry(this->rect());
    }
    event->accept();
}

void TabWidget::updateIndicatorArea(QPoint& p) {
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
        mIndicatorArea = utils::DropArea::TOP;
    } else if (bottomArea) {
        mIndicatorArea = utils::DropArea::BOTTOM;
    } else if (rightArea) {
        mIndicatorArea = utils::DropArea::RIGHT;
    } else if (leftArea) {
        mIndicatorArea = utils::DropArea::LEFT;
    } else if (tabBarArea) {
        mIndicatorArea = utils::DropArea::TABBAR;
    }
}

void TabWidget::updateIndicatorRect() {
    if (mIndicatorArea != utils::DropArea::INVALID) {
        QRect rect = this->rect();
        rect.setTop(tabBar()->rect().height());
        int marginY = rect.height() * 0.4;
        int marginX = rect.width() * 0.4;

        switch (mIndicatorArea) {
        case utils::DropArea::TOP:
            rect.setBottom(marginY);
            break;
        case utils::DropArea::BOTTOM:
            rect.setTop(rect.bottom() - marginY);
            break;
        case utils::DropArea::RIGHT:
            rect.setLeft(rect.right() - marginX);
            break;
        case utils::DropArea::LEFT:
            rect.setRight(rect.left() + marginX);
            break;
        case utils::DropArea::TABBAR: {
            QPoint mousePos = tabBar()->mapFromGlobal(QCursor::pos());
            int index = tabBar()->tabAt(mousePos);
            rect = tabBar()->tabRect(index);
            rect.setRight(rect.left());
            break;
        }
        case utils::DropArea::INVALID:
            break;
        }

        mDrawOverlay->setRect(rect);
    }
}

void TabWidget::onMenuButtonClicked() {
    int runningNumber = MainWindow::instance()->getRunningNumber();
    addTab(new QFrame(this), QString("tab").append(QString::number(runningNumber)));
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

//    if (dropAction == Qt::MoveAction)
//        tabWidget->close();
}
