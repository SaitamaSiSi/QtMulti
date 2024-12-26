#include "voltagemonitoringdatamodel.h"

VoltageMonitoringDataModel::VoltageMonitoringDataModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

VoltageMonitoringDataModel::~VoltageMonitoringDataModel()
{
    m_dataList.clear();
}

int VoltageMonitoringDataModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_dataList.count();
}

QVariant VoltageMonitoringDataModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row < 0 || row >= m_dataList.count())
        return QVariant();

    if (role == EquipDataRoles::DisplayRole) {
        const VoltageMonitoring &equipment = m_dataList[row];
        QVariantMap resultMap;
        resultMap["id"] = equipment.id();
        resultMap["v"] = equipment.v();
        resultMap["a"] = equipment.a();
        resultMap["w"] = equipment.w();
        return QVariant::fromValue(resultMap);
    } else if (role == EquipDataRoles::SelectedRole) {
        return QVariant::fromValue(m_selectedIndex == row);
    }

    return QVariant();
}

QHash<int, QByteArray> VoltageMonitoringDataModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DisplayRole] = "display";
    roles[SelectedRole] = "selected";
    return roles;
}

void VoltageMonitoringDataModel::insert(int index, const VoltageMonitoring &equip)
{
    if(index < 0 || index > m_dataList.count())
    {
        qCritical()<<"insert error, index="<<index;
        return;
    }
    //进行增删改查都需要begin/end，用来发送信号通知关联Model上的view进行更新
    //QModelIndex()得到这个Model中的虚拟rootItem
    beginInsertRows(QModelIndex(), index, index);
    m_dataList.insert(index, equip);
    endInsertRows();
}

void VoltageMonitoringDataModel::remove(int index)
{
    if(index<0||index>m_dataList.count())
    {
        qCritical()<<"remove error, index="<<index;
        return;
    }
    beginRemoveRows(QModelIndex(), index, index);
    m_dataList.removeAt(index);
    endRemoveRows();
}

void VoltageMonitoringDataModel::append(const VoltageMonitoring &equip)
{
    insert(count(), equip);
}

int VoltageMonitoringDataModel::count() const
{
    return rowCount(QModelIndex());
}

void VoltageMonitoringDataModel::setSelectedIndex(int index)
{
    if (index < 0 || index >= m_dataList.count() || index == m_selectedIndex)
        return;

    beginResetModel(); // 通知视图数据将要改变
    m_selectedIndex = index;
    endResetModel(); // 数据改变完成
}

int VoltageMonitoringDataModel::selectedIndex() const
{
    return m_selectedIndex;
}

void VoltageMonitoringDataModel::append(int id, int v, int a, int w)
{
    VoltageMonitoring add(id, v, a, w);
    insert(count(), add);
}

void VoltageMonitoringDataModel::append(const QVariantMap map)
{
    int id = map["id"].toInt();
    int v = map["v"].toInt();
    int a = map["a"].toInt();
    int w = map["w"].toInt();
    VoltageMonitoring add(id, v, a, w);
    insert(count(), add);
}
