//#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QApplication>
#include <QtQml>
#include <QQuickView>

#include "model/equipmentdatamodel.h"
#include "model/person.h"
#include "model/dataentrymodel.h"
#include "model/voltagemonitoringdatamodel.h"
#include "database/sqlitehelper.h"
#include "network/httpserver.h"
#include "encipher/enciphertest.h"
#include "serialport/myserialport.h"

/*
MySerialPort g_mySerialPort;
void showSerialPoetMsg(const QByteArray& hexArray) {
    auto msg = EncipherTest::HexToUchar(hexArray);
    int length = hexArray.length() / 2;
    qDebug() << hexArray <<endl;
    //EncipherTest::print_hex("Main recv:", msg.get(), hexArray.length() / 2);
    unsigned char* rawPtr = msg.get();
    // 打印原始指针指向的数据
    for (int i = 0; i < length; ++i) {
        //qDebug() << "Byte at index" << i << ":" << static_cast<int>(rawPtr[i]);
        qDebug() << "Byte at index" << i << ":" << rawPtr[i];
    }

    if (rawPtr[0] == 0x01) {
        switch(rawPtr[2]) {
        case 0xA1:
            qDebug() << "读卡号(卡序列号)：0xA1" << endl;
            break;
        case 0xA3:
            qDebug() << "读指定块数据：0xA3" << endl;
            break;
        case 0xA4:
            qDebug() << "写数据到指定块：0xA4" << endl;
            break;
        }
    } else if (rawPtr[0] == 0x02) {

    } else if (rawPtr[0] == 0x03) {

    } else if (rawPtr[0] == 0x04) {

    } else {

    }
}
*/

int main(int argc, char *argv[])
{
#if 1
    /* Cpp 测试1 */
    //Person* person = new Person();
    //QString name = person->metaObject()->className(); // "Person"
    //QString name2 = Person::staticMetaObject.className(); // "Person"
    //qDebug() << name << name2 << endl;

    /* SqliteHelper */
    //DB::SqliteHelper sqliteHelper;
    //sqliteHelper.myTest();

    /* HttpSever */
    HttpServer httpServer(8080);
    httpServer.start();

    /* EncipherTest */
    //EncipherTest encipherTest;
    //encipherTest.monocypherTest();
    //encipherTest.Base64Test();
    //encipherTest.rsaTest();
#endif
    //QObject::connect(&g_mySerialPort, &MySerialPort::readMsgSignal, showSerialPoetMsg);
    //g_mySerialPort.openPort("/dev/ttyv0");

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // QGuiApplication app(argc, argv);
    QApplication app(argc, argv);

    // 注册自定义类
    qmlRegisterType<Equipment>("Equipment", 1, 0, "Equipment");
    qmlRegisterType<EquipmentDataModel>("EquipmentDataModel", 1, 0, "EquipmentDataModel");
    qmlRegisterType<DataEntryModel>("MyDataEntryModel",1,0,"DataEntryModel");
    qmlRegisterType<VoltageMonitoringDataModel>("VoltageMonitoringDataModel",1,0,"VoltageMonitoringDataModel");

    QQmlApplicationEngine engine;

    // 串口
    MySerialPort serial;
    engine.rootContext()->setContextProperty("serial",&serial);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    QString path = engine.offlineStoragePath();
    qDebug() << path;

    // 连接aboutToQuit信号
    QObject::connect(&app, &QApplication::aboutToQuit, []() {
        qDebug() << "应用程序即将退出...";
        // 在这里执行任何必要的清理工作
        //g_mySerialPort.closePort();
    });

    QQuickView view;


    int appretcode = app.exec();
    return appretcode;
}
