#ifndef PUBLISHER_H
#define PUBLISHER_H

//!
//! 代码来源: https://www.cnblogs.com/luoxiang/p/13592134.html
//!
//!

#include <QObject>
#include <QReadWriteLock>
#include <QHash>
#include <QSet>
#include <QStringList>
#include <QScopedPointer>

class Subscriber;

class Publisher : public QObject
{
    Q_OBJECT
public:
    explicit Publisher(QObject *parent = nullptr);
    ~Publisher();

    /*!
     * \brief getInstance 获取Publisher单例指针
     * \return
     */
    static Publisher *self();

    /*!
     * \brief publish 发布主题消息
     * \param topic 主题名称
     * \param msg 消息内容
     */
    void publish(const int &topic, const QVariant &msg);

private:
    Publisher(const Publisher &other);
    Publisher &operator=(const Publisher &other);

    /*!
     * \brief add 订阅者订阅一个主题
     * \param object 订阅者对象指针
     * \param topic 主题名称
     */
    void add(Subscriber *object, const int &topic);

    /*!
     * \brief remove 移除订阅者订阅的对应主题
     * \param object 订阅者对象指针
     * \param topic 主题名称
     */
    void remove(Subscriber *object, const int &topic);

    /*!
     * \brief remove 移除订阅者订阅的所有主题
     * \param object 订阅者对象指针
     */
    void remove(Subscriber *object);

    /*!
     * \brief getTopics 获取订阅者所订阅的主题列表
     * \param object 订阅者对象指针
     * \return
     */
    QList<int> getTopics(Subscriber *object);

    friend class Subscriber;

private:
    static QReadWriteLock sm_readWriteLock;

    static Publisher *sm_instance;
    friend struct QScopedPointerDeleter<Publisher>;

    QHash<Subscriber *, QSet<int> > m_objectTopicHash;
    QHash<int, QVariant> m_topicLastMsgHash;
};

#endif  // PUBLISHER_H
