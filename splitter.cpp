#include "splitter.h"
#include <mainwindow.h>

Splitter::Splitter(QWidget *parent) : QSplitter(parent) {
    setStyleSheet(MainWindow::instance()->styleSheetFile());
}

Splitter::Splitter(Qt::Orientation orientation, QWidget *parent) {
    setStyleSheet(MainWindow::instance()->styleSheetFile());
}
