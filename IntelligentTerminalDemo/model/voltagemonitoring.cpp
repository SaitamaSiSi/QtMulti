#include "voltagemonitoring.h"

VoltageMonitoring::VoltageMonitoring(int id, int v, int a, int w)
    :m_id(id), m_v(v), m_a(a), m_w(w)
{
}


int VoltageMonitoring::id() const
{
    return m_id;
}

void VoltageMonitoring::setId(int id)
{
    if (m_id != id) {
        m_id = id;
    }
}

int VoltageMonitoring::a() const
{
    return m_a;
}

void VoltageMonitoring::setA(int a)
{
    if (m_a != a) {
        m_a = a;
    }
}

int VoltageMonitoring::v() const
{
    return m_v;
}

void VoltageMonitoring::setV(int v)
{
    if (m_v != v) {
        m_v = v;
    }
}

int VoltageMonitoring::w() const
{
    return m_w;
}

void VoltageMonitoring::setW(int w)
{
    if (m_w != w) {
        m_w = w;
    }
}
