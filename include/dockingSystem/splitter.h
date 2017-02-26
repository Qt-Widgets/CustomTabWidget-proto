#pragma once

#include <QSplitter>

class Splitter : public QSplitter
{
    Q_OBJECT
public:
    explicit Splitter(QWidget *parent = 0);
    explicit Splitter(Qt::Orientation orientation, QWidget * parent = 0);
    QList<QWidget*> getWidgets();
    bool hasContainers();
    QList<Splitter*> getSplitters();
};
