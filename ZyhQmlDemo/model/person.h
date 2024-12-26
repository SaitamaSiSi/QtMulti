#ifndef PERSON_H
#define PERSON_H

#include <QObject>

class Person : public QObject
{
    Q_OBJECT // enabled meta object abilities
    // property declarations required for QML
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(Gender gender READ gender WRITE setGender NOTIFY genderChanged)
    // enables enum introspections
    Q_ENUMS(Gender)
public:
    // standard Qt constructor with parent for memory management
    explicit Person(QObject *parent = nullptr);
    enum Gender { Unknown, Male, Female, Other };
    QString name() const;
    Gender gender() const;
public slots: // slots can be connected to signals
    void setName(const QString &);
    void setGender(Gender);
signals: // signals can be emitted
    void nameChanged(const QString &name);
    void genderChanged(Gender gender);
private:
    // data members
    QString m_name;
    Gender m_gender;
};

#endif // PERSON_H
