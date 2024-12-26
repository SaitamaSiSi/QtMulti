#include "publisher.h"
#include "subscriber.h"

#include <QDebug>
#include <QReadLocker>
#include <QWriteLocker>

Publisher *Publisher::sm_instance = nullptr;
QReadWriteLock Publisher::sm_readWriteLock;
Publisher::Publisher(QObject *parent)
    : QObject(parent)
{
    sm_instance = this;
}

Publisher::~Publisher()
{
}

void Publisher::add(Subscriber *object, const int &topic)
{
    QWriteLocker locker(&sm_readWriteLock);

    if (m_objectTopicHash.keys().contains(object))
    {
        auto it = m_objectTopicHash.find(object);
        it.value().insert(topic);
    }
    else
    {
        QSet<int> set = {topic};
        m_objectTopicHash.insert(object, set);

        connect(object, &QObject::destroyed, [=]() {
            remove(object);
        });
    }

    //订阅后将自动发送最后一次主题消息
    if (m_topicLastMsgHash.keys().contains(topic))
    {
        QMetaObject::invokeMethod(object, "topicUpdated", Qt::QueuedConnection,
                                  Q_ARG(int, topic), Q_ARG(QVariant, m_topicLastMsgHash.value(topic)));
    }
}

void Publisher::remove(Subscriber *object, const int &topic)
{
    QWriteLocker locker(&sm_readWriteLock);

    if (m_objectTopicHash.keys().contains(object))
    {
        auto it = m_objectTopicHash.find(object);
        it.value().remove(topic);
    }
}

void Publisher::remove(Subscriber *object)
{
    QWriteLocker locker(&sm_readWriteLock);

    if (m_objectTopicHash.keys().contains(object))
    {
        m_objectTopicHash.remove(object);
    }
}

QList<int> Publisher::getTopics(Subscriber *object)
{
    QReadLocker locker(&sm_readWriteLock);

    if (m_objectTopicHash.keys().contains(object))
    {
        return QList<int>::fromSet(m_objectTopicHash.value(object));
    }

    return QList<int>();
}

Publisher *Publisher::self()
{
    return sm_instance;
}

void Publisher::publish(const int &topic, const QVariant &msg)
{
    QReadLocker locker(&sm_readWriteLock);

    auto it = m_objectTopicHash.constBegin();
    while (it != m_objectTopicHash.constEnd())
    {
        if (it.value().contains(topic))
        {
            QMetaObject::invokeMethod(it.key(), "topicUpdated", Qt::QueuedConnection,
                                      Q_ARG(int, topic), Q_ARG(QVariant, msg));
            m_topicLastMsgHash.insert(topic, msg);

        }

        ++it;
    }
}
