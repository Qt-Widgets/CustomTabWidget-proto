#include "splitter.h"
#include <mainwindow.h>

Splitter::Splitter(QWidget *parent) : QSplitter(parent) {
    setStyleSheet(MainWindow::instance()->styleSheetFile());
}

Splitter::Splitter(Qt::Orientation orientation, QWidget *parent) {
    setStyleSheet(MainWindow::instance()->styleSheetFile());
}

///
/// \brief Splitter::setHandlePos
/// \param location, from 0 to 1 in the allowed range.
/// \param index
///
void Splitter::setHandlePos(float location, int index) {
//    int pos;
//    pos = h
//    pos *= location;
//    moveSplitter(pos, index);
}
