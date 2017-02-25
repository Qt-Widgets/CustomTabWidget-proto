#include <mainwindow.h>
#include <include/dockingSystem/splitter.h>

Splitter::Splitter(QWidget *parent) : QSplitter(parent) {
    setStyleSheet(MainWindow::instance()->styleSheetFile());
}

Splitter::Splitter(Qt::Orientation orientation, QWidget *parent) {
    setStyleSheet(MainWindow::instance()->styleSheetFile());
}

QList<QWidget*> Splitter::getWidgets() {
    QList<QWidget*> widgets;
    for (int i = 0; i < count(); i++) {
        widgets.append(widget(i));
    }
    return widgets;
}
