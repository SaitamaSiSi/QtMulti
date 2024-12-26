#include "myserialport.h"

#include <QDebug>
#include <QThread>
#include <QSerialPortInfo>

MySerialPort::MySerialPort(QObject *parent) : QObject(parent),
    serialPort(new QSerialPort())
{
    qDebug() << __FUNCTION__ << "Thread ID:" << QThread::currentThreadId();
}

MySerialPort::~MySerialPort()
{
    qDebug() << __FUNCTION__ << "Thread ID:" << QThread::currentThreadId();
    closePort();
    delete serialPort;
}

bool MySerialPort::openPort(const QString &portName, int baudRate, QSerialPort::DataBits dataBits, QSerialPort::Parity parity, QSerialPort::StopBits stopBits, QSerialPort::FlowControl flowControl)
{
    qDebug() << __FUNCTION__ << "Thread ID:" << QThread::currentThreadId();
    serialPort->setPortName(portName);
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(dataBits);
    serialPort->setParity(parity);
    serialPort->setStopBits(stopBits);
    serialPort->setFlowControl(flowControl);

    bool isOk = serialPort->open(QIODevice::OpenModeFlag::ReadWrite);
    if (isOk) {
        //QObject::connect(serialPort, SIGNAL(readyRead()),this, SLOT(readMsgSlot()));
        connect(serialPort, &QSerialPort::readyRead, this, &MySerialPort::readMsgSlot);
        connect(serialPort, &QSerialPort::errorOccurred, this, &MySerialPort::errorOccurredSlot);
    }
    return isOk;
}

void MySerialPort::closePort()
{
    qDebug() << __FUNCTION__ << "Thread ID:" << QThread::currentThreadId();
    if (serialPort && serialPort->isOpen()) {
        disconnect(serialPort, &QSerialPort::readyRead, this, &MySerialPort::readMsgSlot);
        disconnect(serialPort, &QSerialPort::errorOccurred, this, &MySerialPort::errorOccurredSlot);
        serialPort->close();
    }
}

bool MySerialPort::sendMessage(const QByteArray &message)
{
    qDebug() << __FUNCTION__ << "Thread ID:" << QThread::currentThreadId();
    if (serialPort->isOpen()) {
        qint64 writeBytes = serialPort->write(message);
        qDebug() << "write length:" + QString::number(writeBytes);
        return serialPort->waitForBytesWritten();
    }
    return false;
}

QByteArray MySerialPort::readMessage()
{
    qDebug() << __FUNCTION__ << "Thread ID:" << QThread::currentThreadId();
    QByteArray data;
    if (serialPort->isOpen()) {
        qint64 num = serialPort->bytesAvailable();
        qDebug() << "bytesAvailable: " << num;
        if (num > 0) {
            data = serialPort->readAll();  // 读取所有可用数据
        }
    }
    return data;
}

QList<QString> MySerialPort::getAvaliablePorts()
{
    qDebug() << __FUNCTION__ << "Thread ID:" << QThread::currentThreadId();
    QList<QString> res;
    QList<QSerialPortInfo> list = QSerialPortInfo::availablePorts();
    foreach (QSerialPortInfo info, list) {
        qDebug() << "portName:" << info.portName();
        res.append(info.portName());
    }
    return res;
}

void MySerialPort::readMsgSlot()
{
    qDebug() << __FUNCTION__ << "Thread ID:" << QThread::currentThreadId();
    while (serialPort->bytesAvailable()) {
        QByteArray message = serialPort->readAll();  // 读取所有可用数据
        emit readMsgSignal(message);  // 发送信号
    }
}

void MySerialPort::errorOccurredSlot()
{
    qDebug() << __FUNCTION__ << "Thread ID:" << QThread::currentThreadId();
    qDebug() << serialPort->errorString();
}


