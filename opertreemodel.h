#ifndef OPERTREEMODEL_H
#define OPERTREEMODEL_H

#include "datasource.h"
#include <QAbstractItemModel>
#include <QObject>

class OperTreeModel : public QAbstractItemModel
{
    Q_OBJECT

    DataSource *treeData;
public:
    explicit OperTreeModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
};

#endif // OPERTREEMODEL_H
