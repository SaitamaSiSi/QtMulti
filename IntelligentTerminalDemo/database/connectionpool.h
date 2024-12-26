#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H

#include <QtSql>
#include <QQueue>
#include <QString>
#include <QMutex>
#include <QMutexLocker>
#include <QDebug>
#include <atomic>

namespace DB {

    class ConnectionPool : public QObject
    {
    public:
        static QSqlDatabase openConnection();                  // 获取数据库连接
        static void closeConnection(QSqlDatabase connection);  //释放数据库连接回连接池
        static void release();                                 //关闭所有的数据库连接

        static std::atomic<bool> enable_pool;

    private:
        static ConnectionPool &getInstance();

        ConnectionPool();
        ~ConnectionPool();
        ConnectionPool(const ConnectionPool &other);
        ConnectionPool &operator=(const ConnectionPool &other);
        QSqlDatabase getConnection(const QString &connectionName);  // 创建数据库连接

        //获取连接总数
        static int getConnectionCount(const ConnectionPool &pool)
        {
            return pool.unusedConnectionNames.size() + pool.usedConnectionNames.size();
        }

        //数据库连接是否用尽
        static bool conectIsExhaustion(const ConnectionPool &pool)
        {
            return (pool.unusedConnectionNames.size() == 0 && getConnectionCount(pool) == pool.maxConnectionCount);
        }

        QQueue<QString> usedConnectionNames;    // 已使用的数据库连接名
        QQueue<QString> unusedConnectionNames;  // 未使用的数据库连接名

        // 数据库信息
        QString hostName;
        QString databaseName;
        QString username;
        QString password;
        QString databaseType;

        bool testOnBorrow;        // 取得连接的时候验证连接是否有效
        QString testOnBorrowSql;  // 测试访问数据库的 SQL

        unsigned long maxWaitTime;   // 获取连接最大等待时间
        unsigned long waitInterval;  // 尝试获取连接时等待间隔时间
        int maxConnectionCount;      // 最大连接数

        static QMutex mutex;
        static QWaitCondition waitConnection;
        static ConnectionPool *instance;
    };

}  //namespace DB
#endif  // CONNECTIONPOOL_H
