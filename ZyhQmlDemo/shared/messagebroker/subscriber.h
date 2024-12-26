#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <QObject>
#include <QStringList>

class Subscriber : public QObject
{
    Q_OBJECT
public:
    explicit Subscriber(QObject *parent = nullptr);

    /*!
     * \brief subscribe 订阅主题
     * \param topic 主题名称
     */
    void subscribe(const int &topic);

    /*!
     * \brief unSubscribe 取消订阅
     * \param topic 主题名称
     */
    void unSubscribe(const int &topic);

    /*!
     * \brief clearSubscribedTopics 取消所有已订阅主题
     */
    void clearSubscribedTopics();

    /*!
     * \brief topics 获取已订阅的主题列表
     * \return 主题列表
     */
    QList<int> topics();

signals:
    /*!
     * \brief topicUpdated 主题消息更新信号
     * \param topic 主题名称
     * \param var 消息内容
     */
    void topicUpdated(const int &topic, const QVariant &msg);
};

#endif  // SUBSCRIBER_H
