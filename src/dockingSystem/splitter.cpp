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

bool Splitter::hasContainers() {
    QList<QWidget*> widgets = getWidgets();
    for (QWidget* item : widgets) {
        TabWidgetContainer* container = static_cast<TabWidgetContainer*>(item);
        if (container) {
            return true;
        }
    }
    return false;
}

QList<Splitter*> Splitter::getSplitters() {
    QList<QWidget*> widgets = getWidgets();
    QList<Splitter*> splitters;
    for (QWidget* item : widgets) {
        Splitter* splitter = static_cast<Splitter*>(item);
        if (splitter) {;
            splitters.append(splitter);
        }
    }
    return splitters;
}


