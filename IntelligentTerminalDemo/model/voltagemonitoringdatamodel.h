#ifndef VOLTAGEMONITORINGDATAMODEL_H
#define VOLTAGEMONITORINGDATAMODEL_H

#include <QtCore>
#include "voltagemonitoring.h"

class VoltageMonitoringDataModel : public QAbstractListModel
{
    Q_OBJECT
    //Qml使用C++中的枚举数据
    Q_ENUMS(EquipDataRoles)
public:
    VoltageMonitoringDataModel(QObject *parent=nullptr);
    ~VoltageMonitoringDataModel();
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
    Q_INVOKABLE void insert(int, const VoltageMonitoring &);
    Q_INVOKABLE void remove(int);
    Q_INVOKABLE void append(const VoltageMonitoring &);
    Q_INVOKABLE int count() const;
    Q_INVOKABLE void setSelectedIndex(int index);
    Q_INVOKABLE int selectedIndex() const;
    Q_INVOKABLE void append(int, int, int, int);
    Q_INVOKABLE void append(const QVariantMap map);
private:
    //model数据集
    QList<VoltageMonitoring> m_dataList;
    int m_selectedIndex;
};

#endif // VOLTAGEMONITORINGDATAMODEL_H
