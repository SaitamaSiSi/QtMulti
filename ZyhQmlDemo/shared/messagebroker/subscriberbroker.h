#ifndef SUBSCRIBERBROKER_H
#define SUBSCRIBERBROKER_H

#include <QObject>
#include "subscriber.h"

class SubScriberBroker : public QObject
{
    Q_OBJECT
public:
    explicit SubScriberBroker(QObject *parent = nullptr, bool init_sub = true);

signals:

protected slots:
    //!处理订阅消息
    virtual void handle_subcriber_message(const int &topic, const QVariant &msg) = 0;

protected:
    void init_subcriber();

protected:
    Subscriber *m_subcriber{nullptr};  //消息订阅
};

#endif  // SUBSCRIBERBROKER_H
