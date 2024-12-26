#ifndef SQLITEHELPER_H
#define SQLITEHELPER_H

#include <QObject>

namespace DB {

class SqliteHelper : public QObject
{
    Q_OBJECT
public:
    explicit SqliteHelper(QObject *parent = nullptr);
    ~SqliteHelper();
    void myTest();
    void openDb();
    void openTable();
private:
protected:
signals:
};

}
#endif // SQLITEHELPER_H
