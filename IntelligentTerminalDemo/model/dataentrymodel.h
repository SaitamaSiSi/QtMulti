#ifndef DATAENTRYMODEL_H
#define DATAENTRYMODEL_H

#include <QtCore>
#include <QtGui>

class DataEntryModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit DataEntryModel(QObject *parent = nullptr);
    ~DataEntryModel();
    enum DataRoles {
        DisplayRole = Qt::DisplayRole,
        SelectedRole = Qt::UserRole + 1 // 自定义角色用于表示选中状态
    };
public: // QAbstractItemModel interface
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

    void setSelectedIndex(int index); // 设置选中项
    int selectedIndex() const; // 获取选中项
private:
    QList<QString> m_data;
    int m_selectedIndex;
};
#endif // DATAENTRYMODEL_H
