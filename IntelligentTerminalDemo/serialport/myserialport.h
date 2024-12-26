#ifndef MYSERIALPORT_H
#define MYSERIALPORT_H

#include <QObject>
#include <QSerialPort>

#include "encipher/enciphertest.h"

class MySerialPort : public QObject
{
    Q_OBJECT
public:
    explicit MySerialPort(bool isConnect = true, QObject *parent = nullptr);
    ~MySerialPort();

    Q_PROPERTY(short testValue READ testValue WRITE setTestValue NOTIFY testValueChanged)

    Q_INVOKABLE bool openPort(const QString &portName,
                              int baudRate = QSerialPort::Baud115200,
                              QSerialPort::DataBits dataBits = QSerialPort::DataBits::Data8,
                              QSerialPort::Parity parity = QSerialPort::Parity::NoParity,
                              QSerialPort::StopBits stopBits = QSerialPort::StopBits::OneStop,
                              QSerialPort::FlowControl flowControl = QSerialPort::FlowControl::NoFlowControl);
    Q_INVOKABLE void closePort();
    Q_INVOKABLE bool sendMessage(const QByteArray &message);
    Q_INVOKABLE QByteArray readMessage();
    Q_INVOKABLE QList<QString> getAvaliablePorts();


    /* MyTest */
    Q_INVOKABLE bool send_ic_test();
    Q_INVOKABLE void icDataVerifyTest(const QByteArray &);
    QByteArray get_ic_test1();
    void cmdReadId();
    void cmdReadBlockHex();
    void cmdWriteBlockHex();
    void cmdInitCard();
    void cmdReadCard();
    void cmdCardAdd();
    void cmdCardDel();
    bool isOpen();

    short testValue() const;
    void setTestValue(short newTestValue);

public slots:
    void readMsgSlot();
    void errorOccurredSlot();

private:
    bool isConnect = true;
    QSerialPort *serialPort;
    EncipherTest encipherHelper;

    short mTestValue{1};

signals:
    void readMsgSignal(const QByteArray &message);

    void testValueChanged();
};

#endif // MYSERIALPORT_H
