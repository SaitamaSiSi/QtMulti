#include "person.h"

Person::Person(QObject *parent)
    : QObject(parent)
    , m_gender(Person::Unknown)
{

}

QString Person::name() const
{
    return m_name;
}

void Person::setName(const QString &name)
{
    if (m_name != name) // guard
    {
        m_name = name;
        emit nameChanged(m_name);
    }
}

Person::Gender Person::gender() const
{
    return m_gender;
}

void Person::setGender(Gender)
{
    // TODO
}
