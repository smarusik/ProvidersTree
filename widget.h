#ifndef WIDGET_H
#define WIDGET_H

#include "opertreemodel.h"
#include <QTreeView>
#include <QWidget>
#include <QHBoxLayout>

class Widget : public QWidget
{
    Q_OBJECT
    OperTreeModel* operModel;
    QTreeView *operTree;
    QHBoxLayout *mainLayout;
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
};
#endif // WIDGET_H
