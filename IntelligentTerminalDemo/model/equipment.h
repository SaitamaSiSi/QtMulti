#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <QObject>

class Equipment : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString equipId READ equipId WRITE setEquipId)
    Q_PROPERTY(QString phase READ phase WRITE setPhase)
    Q_PROPERTY(QString type READ type WRITE setType)
    Q_PROPERTY(QString identification READ identification WRITE setIdentification)
public:
    Equipment(QObject *parent = nullptr);
    Equipment(const QString &, const QString &, const QString &, const QString &);
    QString equipId() const;
    void setEquipId(const QString &);
    QString phase() const;
    void setPhase(const QString &);
    QString type() const;
    void setType(const QString &);
    QString identification() const;
    void setIdentification(const QString &);
    Equipment(const Equipment &);
    Equipment operator =(const Equipment &);
signals:
private:
    QString m_id;
    QString m_phase;
    QString m_type;
    QString m_identification;
};

#endif // EQUIPMENT_H
