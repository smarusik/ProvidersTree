#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    operModel=new OperTreeModel();
    mainLayout=new QHBoxLayout(this);
    setLayout(mainLayout);
    operTree=new QTreeView(this);
    operTree->setHeaderHidden(true);
    mainLayout->addWidget(operTree);
    operTree->setModel(operModel);
}

Widget::~Widget() {}
