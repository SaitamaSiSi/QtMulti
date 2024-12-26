#ifndef EQUIPMENTDATAMODEL_H
#define EQUIPMENTDATAMODEL_H

#include <QtCore>
#include "equipment.h"

class EquipmentDataModel : public QAbstractListModel
{
    Q_OBJECT
    //Qml使用C++中的枚举数据
    Q_ENUMS(EquipDataRoles)
public:
    EquipmentDataModel(QObject *parent=nullptr);
    ~EquipmentDataModel();
    enum EquipDataRoles{
        DisplayRole = Qt::DisplayRole,
        SelectedRole = Qt::UserRole + 1 // 自定义角色用于表示选中状态
    };
    //返回Model中的数据个数
    int rowCount(const QModelIndex &parent) const;
    //访问每个列表项的接口，通过index，role来访问对应的属性
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    //设置role的别名
    QHash<int, QByteArray> roleNames() const;
    //增删改查的方法
    Q_INVOKABLE void insert(int, const Equipment &);
    Q_INVOKABLE void remove(int);
    Q_INVOKABLE int count() const;
    Q_INVOKABLE void setSelectedIndex(int index);
    Q_INVOKABLE int selectedIndex() const;
    Q_INVOKABLE void append(const QString &, const QString &, const QString &, const QString &);
    Q_INVOKABLE void append(const QVariantMap map);
private:
    //model数据集
    QList<Equipment> m_dataList;
    int m_selectedIndex;
};

#endif // EQUIPMENTDATAMODEL_H
