#include "equipment.h"

Equipment::Equipment(QObject *parent) : QObject(parent)
{
}

Equipment::Equipment(const QString &id, const QString &phase, const QString &type, const QString &identification)
    :m_id(id), m_phase(phase), m_type(type), m_identification(identification)
{
}

Equipment::Equipment(const Equipment &equip)
{
    m_id = equip.equipId();
    m_phase = equip.phase();
    m_type = equip.type();
    m_identification = equip.identification();
}

Equipment Equipment::operator = (const Equipment &equip)
{
    Equipment ret;
    ret.setEquipId(equip.equipId());
    ret.setPhase(equip.phase());
    ret.setType(equip.type());
    ret.setIdentification(equip.identification());
    return ret;
}

QString Equipment::equipId() const
{
    return m_id;
}

void Equipment::setEquipId(const QString & id)
{
    if (m_id != id) {
        m_id = id;
    }
}

QString Equipment::phase() const
{
    return m_phase;
}

void Equipment::setPhase(const QString & phase)
{
    if (m_phase != phase) {
        m_phase = phase;
    }
}

QString Equipment::type() const
{
    return m_type;
}

void Equipment::setType(const QString & type)
{
    if (m_type != type) {
        m_type = type;
    }
}

QString Equipment::identification() const
{
    return m_identification;
}

void Equipment::setIdentification(const QString & identification)
{
    if (m_identification != identification) {
        m_identification = identification;
    }
}
