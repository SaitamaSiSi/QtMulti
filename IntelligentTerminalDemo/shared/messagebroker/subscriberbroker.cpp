#include "subscriberbroker.h"

SubScriberBroker::SubScriberBroker(QObject *parent, bool init_sub)
    : QObject(parent)
{
    if (init_sub) init_subcriber();
}

void SubScriberBroker::init_subcriber()
{
    m_subcriber = new Subscriber(this);
    connect(m_subcriber, &Subscriber::topicUpdated, this, &SubScriberBroker::handle_subcriber_message);
}
