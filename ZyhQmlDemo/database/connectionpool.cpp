#include "connectionpool.h"

#include <QCoreApplication>

namespace DB {

    QMutex ConnectionPool::mutex;
    QWaitCondition ConnectionPool::waitConnection;
    ConnectionPool* ConnectionPool::instance = nullptr;

    std::atomic<bool> ConnectionPool::enable_pool = ATOMIC_VAR_INIT(true);

    ConnectionPool::ConnectionPool()
    {
        // 创建数据库连接的这些信息在实际开发的时都需要通过读取配置文件得到，
        // 这里为了演示方便所以写死在了代码里。
        databaseName = "./sqlite.db";
        databaseType = "QSQLITE";
        testOnBorrow = true;
        testOnBorrowSql = "select 1";

        maxWaitTime = 5000;
        waitInterval = 200;
        maxConnectionCount = 10;
    }

    /**
     * @brief 从连接池获取一个连接
     * @return 成功返回可用连接，否则返回QSqlDatabase()--不可用
     */
    QSqlDatabase ConnectionPool::openConnection()
    {
        ConnectionPool& pool = ConnectionPool::getInstance();
        QString connectionName;

        if (!enable_pool.load())
        {
            return QSqlDatabase();
        }

        QMutexLocker locker(&mutex);

        // 如果连接已经用完，等待 waitInterval 毫秒看看是否有可用连接，最长等待 maxWaitTime 毫秒
        for (unsigned long i = 0; i < pool.maxWaitTime && conectIsExhaustion(pool); i += pool.waitInterval)
        {
            waitConnection.wait(&mutex, pool.waitInterval);
        }

        if (pool.unusedConnectionNames.size() > 0)
        {
            // 有已经回收的连接，复用它们
            connectionName = pool.unusedConnectionNames.dequeue();
        }
        else if (getConnectionCount(pool) < pool.maxConnectionCount)
        {
            // 没有已经回收的连接，但是没有达到最大连接数，则创建新的连接
            connectionName = QString("Connection-%1").arg(getConnectionCount(pool) + 1);
        }
        else
        {
            // 已经达到最大连接数
            qWarning() << "Cannot create more connections.";
            return QSqlDatabase();
        }

        // 创建连接
        QSqlDatabase db = pool.getConnection(connectionName);

        // 有效的连接才放入 usedConnectionNames
        if (db.isOpen())
        {
            pool.usedConnectionNames.enqueue(connectionName);
        }

        return db;
    }

    /**
     * @brief 释放数据库连接回连接池
     * @param connection 即将要释放的连接
     */
    void ConnectionPool::closeConnection(QSqlDatabase connection)
    {
        ConnectionPool& pool = ConnectionPool::getInstance();
        QString connectionName = connection.connectionName();

        // 如果是我们创建的连接，从 used 里删除，放入 unused 里
        if (pool.usedConnectionNames.contains(connectionName))
        {
            QMutexLocker locker(&mutex);
            //qDebug()<<"remove "<<connectionName;
            pool.usedConnectionNames.removeOne(connectionName);
            pool.unusedConnectionNames.enqueue(connectionName);
            waitConnection.wakeOne();
        }
    }

    /**
     * @brief 释放所有连接
     */
    void ConnectionPool::release()
    {
        QMutexLocker locker(&mutex);
        if (instance) delete instance;
        instance = nullptr;
    }

    /**
     * @brief 销毁连接池的时候删除所有的连接
     */
    ConnectionPool::~ConnectionPool()
    {
        foreach (QString connectionName, usedConnectionNames)
        {
            QSqlDatabase::removeDatabase(connectionName);
        }

        foreach (QString connectionName, unusedConnectionNames)
        {
            QSqlDatabase::removeDatabase(connectionName);
        }
    }

    /**
     * @brief 单例模式获取连接池实体
     * @return
     */
    ConnectionPool& ConnectionPool::getInstance()
    {
        if (nullptr == instance)
        {
            QMutexLocker locker(&mutex);

            if (nullptr == instance)
            {
                instance = new ConnectionPool();
            }
        }

        return *instance;
    }

    /**
     * @brief 创建一个连接
     * @param connectionName 连接名
     * 如果连接名已经创建过，那么返回一个复用的连接，否则返回新连接
     * @return 成功返回可用连接，否则返回QSqlDatabase()--不可用
     */
    QSqlDatabase ConnectionPool::getConnection(const QString& connectionName)
    {
        // 连接已经创建过了，复用它，而不是重新创建
        if (QSqlDatabase::contains(connectionName))
        {
            QSqlDatabase db1 = QSqlDatabase::database(connectionName);

            // 返回连接前访问数据库，如果连接断开，重新建立连接
            if (testOnBorrow)
            {
                //qDebug() << "Test connection on borrow, execute:" << testOnBorrowSql << ", for" << connectionName;
                QSqlQuery query(testOnBorrowSql, db1);

                if (query.lastError().type() != QSqlError::NoError && !db1.open())
                {
                    qDebug() << "Open datatabase error:" << db1.lastError().text();
                    return QSqlDatabase();
                }
            }

            return db1;
        }

        // 创建一个新的连接
        QSqlDatabase db = QSqlDatabase::addDatabase(databaseType, connectionName);
        //db.setHostName("localhost");
        //db.setUserName("gjktouch");
        //db.setPassword("ebos62798988");
        db.setDatabaseName(databaseName);

        if (!db.open())
        {
            qWarning() << "Open datatabase error:" << db.lastError().text();
            return QSqlDatabase();
        }

        return db;
    }

}  // namespace DB
