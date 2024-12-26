#ifndef VOLTAGEMONITORING_H
#define VOLTAGEMONITORING_H

class VoltageMonitoring
{
public:
    VoltageMonitoring(int, int, int, int);
    int id() const;
    void setId(int);
    int a() const;
    void setA(int);
    int v() const;
    void setV(int);
    int w() const;
    void setW(int);
private:
    int m_id;
    int m_v;
    int m_a;
    int m_w;
};

#endif // VOLTAGEMONITORING_H
