#include "opertreemodel.h"
#include "datasource.h"
#include <QFont>

OperTreeModel::OperTreeModel(QObject *parent)
    : QAbstractItemModel{parent}
{
    treeData=new SqlDataSource("system.db");
    treeData->aquireData();
}


QModelIndex OperTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    TreeItem *elem;

    if(!parent.isValid()) //parent is root
    {
        elem=treeData->countryForRowColumn(row,column);
    }
    else
    {
        elem=treeData->operForRowColumn(row,column, parent.row());
    }

    if(elem)
        return createIndex(row,column,elem);
    else
        return QModelIndex();
}

QModelIndex OperTreeModel::parent(const QModelIndex &child) const
{
    if(!child.isValid())
        return QModelIndex();

    TreeItem *item=static_cast<TreeItem*>(child.internalPointer());

    if(item->parent) //we have operator
    {
        int itemRow=treeData->rowForCountry(item->parent->code());
        if(itemRow>0)
            return createIndex(itemRow, 0, item->parent);
    }

    return QModelIndex();
}

int OperTreeModel::rowCount(const QModelIndex &parent) const
{
    if(parent.column()>0)
        return 0;

    if(!parent.isValid()) //root node - count countries
        return treeData->countCountries();

    TreeItem *item=static_cast<TreeItem*>(parent.internalPointer());
    if(!item)
        return 0;
    else //we have country
        return item->countChildren();
}

int OperTreeModel::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid() && parent.column()!=0)
        return 0;
    else
        return 1;
}

QVariant OperTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TreeItem *item=static_cast<TreeItem*>(index.internalPointer());

    switch (role) {
    case Qt::DisplayRole:
    {
        return item->displayName();
    }
    case Qt::DecorationRole:
    {
        return item->decoration();
    }
    case Qt::FontRole:
    {
        QFont f;
        if(!index.parent().isValid())//countries
            f.setBold(true);

        return f;
    }
    default:
        return QVariant();
    }
}

Qt::ItemFlags OperTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index)|Qt::ItemIsEnabled;
}
