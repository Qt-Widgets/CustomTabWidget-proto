#include <mainwindow.h>
#include <include/dockingSystem/splitter.h>

Splitter::Splitter(QWidget *parent) : QSplitter(parent) {
    setStyleSheet(MainWindow::instance()->styleSheetFile());
}

Splitter::Splitter(Qt::Orientation orientation, QWidget *parent) {
    setStyleSheet(MainWindow::instance()->styleSheetFile());
}
