#include "dataentrymodel.h"

DataEntryModel::DataEntryModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_data.append("red");
    m_data.append("green");
    m_data.append("blue");
}

DataEntryModel::~DataEntryModel()
{
    m_data.clear();
}

int DataEntryModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.count();
}

QVariant DataEntryModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_data.count())
        return QVariant();

    if (role == Qt::DisplayRole) {
        return m_data[index.row()];
    }

    return QVariant();
}

QHash<int, QByteArray> DataEntryModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DisplayRole] = "display";
    roles[SelectedRole] = "selected";
    return roles;
}

void DataEntryModel::setSelectedIndex(int index)
{
    if (index < 0 || index >= m_data.count() || index == m_selectedIndex)
        return;

    beginResetModel(); // 通知视图数据将要改变
    m_selectedIndex = index;
    endResetModel(); // 数据改变完成
}

int DataEntryModel::selectedIndex() const
{
    return m_selectedIndex;;
}

