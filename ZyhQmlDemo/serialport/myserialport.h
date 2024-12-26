#ifndef MYSERIALPORT_H
#define MYSERIALPORT_H

#include <QObject>
#include <QSerialPort>

class MySerialPort : public QObject
{
    Q_OBJECT
public:
    explicit MySerialPort(QObject *parent = nullptr);
    ~MySerialPort();

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

public slots:
    void readMsgSlot();
    void errorOccurredSlot();

private:
    QSerialPort *serialPort;

signals:
    void readMsgSignal(const QByteArray &message);
};

#endif // MYSERIALPORT_H
