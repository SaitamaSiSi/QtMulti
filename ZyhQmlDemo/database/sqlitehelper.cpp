#include "sqlitehelper.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

#include "connectionpool.h"

namespace DB {

SqliteHelper::SqliteHelper(QObject *parent) : QObject(parent)
{
}

SqliteHelper::~SqliteHelper()
{
    ConnectionPool::release();
}

void SqliteHelper::myTest()
{
    QSqlDatabase db = ConnectionPool::openConnection();

    QSqlQuery dropTable(db);
    if (dropTable.exec("DROP TABLE IF EXISTS my_equip")) {
        qDebug() << "drop success";
    } else {
        qDebug() << "drop failed: " << dropTable.lastError();
    }

    QSqlQuery createTable(db);
    // (id INTEGER PRIMARY KEY AUTOINCREMENT)
    if (!createTable.exec("CREATE TABLE IF NOT EXISTS my_equip ("
                          "id INTEGER PRIMARY KEY, "
                          "name TEXT NOT NULL, "
                          "age TEXT);")) {
        qDebug() << "create failed: " << createTable.lastError();
    } else {
        qDebug() << "create success";
    }

    QStringList names;
    QStringList ages;
    names << "Alice" << "Bob" << "Charlie";
    ages << "30" << "25" << "35";
    // 执行SQL语句前，确保事务开始
    QSqlQuery transaction(db);
    transaction.exec("BEGIN TRANSACTION;");
    // SQL插入语句，假设我们有一个用户表，字段包括id, name, age
    QString insertSql = "INSERT INTO my_equip (id, name, age) VALUES (?, ?, ?);";
    QSqlQuery insertQuery(db);
    insertQuery.prepare(insertSql);
    // 插入多条数据
    bool isOk = true;
    for (int i = 0; i < names.size(); ++i) {
        insertQuery.bindValue(0, i + 1);
        insertQuery.bindValue(1, names[i]);
        insertQuery.bindValue(2, ages[i]);
        if (!insertQuery.exec()) {
            qDebug() << "插入失败:" << insertQuery.lastError();
            // 如果插入失败，回滚事务
            transaction.exec("ROLLBACK;");
            isOk = false;
        }
    }
    // 提交事务
    if (isOk) {
        transaction.exec("COMMIT;");
    }

    QSqlQuery deleteQuery(db);
    deleteQuery.prepare("DELETE FROM my_equip WHERE id = ?");
    deleteQuery.addBindValue(2);
    if (deleteQuery.exec()) {
        qDebug() << "delete success";
    } else {
        qDebug() << "delete failed: " << deleteQuery.lastError();
    }

    QSqlQuery updateQuery(db);
    updateQuery.prepare("UPDATE my_equip SET name = ? WHERE id = ?");
    updateQuery.addBindValue("Jack");
    updateQuery.addBindValue(1);
    if (updateQuery.exec()) {
        qDebug() << "update success";
    } else {
        qDebug() << "update failed: " << updateQuery.lastError();
    }


    QSqlQuery selectQuery(db);
    if (!selectQuery.exec("SELECT * FROM my_equip WHERE id > 0")) {
        qDebug() << " query failed: " << selectQuery.lastError();
    } else {
        while(selectQuery.next()) {
            qDebug() << selectQuery.value("id").toString()
                     << selectQuery.value("name").toString()
                     << selectQuery.value("age").toString();
        }
    }

    ConnectionPool::closeConnection(db);
}

}
