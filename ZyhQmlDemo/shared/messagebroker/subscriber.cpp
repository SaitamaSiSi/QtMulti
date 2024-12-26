#include "subscriber.h"
#include "publisher.h"

#include <QDebug>

Subscriber::Subscriber(QObject *parent)
    : QObject(parent)
{
}

void Subscriber::subscribe(const int &topic)
{
    Publisher *publiser = Publisher::self();
    publiser->add(this, topic);
}

void Subscriber::unSubscribe(const int &topic)
{
    Publisher *publiser = Publisher::self();
    publiser->remove(this, topic);
}

void Subscriber::clearSubscribedTopics()
{
    Publisher *publiser = Publisher::self();
    publiser->remove(this);
}

QList<int> Subscriber::topics()
{
    Publisher *publiser = Publisher::self();
    return publiser->getTopics(this);
}
