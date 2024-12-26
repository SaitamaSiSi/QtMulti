#include "equipmentdatamodel.h"

EquipmentDataModel::EquipmentDataModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

EquipmentDataModel::~EquipmentDataModel()
{
    m_dataList.clear();
}

int EquipmentDataModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_dataList.count();
}

QVariant EquipmentDataModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row < 0 || row >= m_dataList.count())
        return QVariant();

    if (role == EquipDataRoles::DisplayRole) {
        const Equipment &equipment = m_dataList[row];
        QVariantMap resultMap;
        resultMap["equipId"] = equipment.equipId();
        resultMap["phase"] = equipment.phase();
        resultMap["type"] = equipment.type();
        resultMap["identification"] = equipment.identification();
        return QVariant::fromValue(resultMap);
    } else if (role == EquipDataRoles::SelectedRole) {
        return QVariant::fromValue(m_selectedIndex == row);
    }

    return QVariant();
}

QHash<int, QByteArray> EquipmentDataModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DisplayRole] = "display";
    roles[SelectedRole] = "selected";
    return roles;
}

void EquipmentDataModel::insert(int index, const Equipment &equip)
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

void EquipmentDataModel::remove(int index)
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

int EquipmentDataModel::count() const
{
    return rowCount(QModelIndex());
}

void EquipmentDataModel::setSelectedIndex(int index)
{
    if (index < 0 || index >= m_dataList.count() || index == m_selectedIndex)
        return;

    beginResetModel(); // 通知视图数据将要改变
    m_selectedIndex = index;
    endResetModel(); // 数据改变完成
}

int EquipmentDataModel::selectedIndex() const
{
    return m_selectedIndex;
}

void EquipmentDataModel::append(const QString &id, const QString &phase, const QString &type, const QString &identification)
{
    Equipment add(id, phase, type, identification);
    insert(count(), add);
}

void EquipmentDataModel::append(const QVariantMap map)
{
    QString id = map["id"].toString();
    QString phase = map["phase"].toString();
    QString type = map["type"].toString();
    QString identification = map["identification"].toString();
    Equipment add(id, phase, type, identification);
    insert(count(), add);
}
