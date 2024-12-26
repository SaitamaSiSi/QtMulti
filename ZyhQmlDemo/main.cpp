#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "model/person.h"
#include "model/dataentrymodel.h"
#include "database/sqlitehelper.h"
#include "network/httpserver.h"
#include "encipher/enciphertest.h"
#include "serialport/myserialport.h"

/* QML练习Demo */
int main(int argc, char *argv[])
{
#if 0
    /* Cpp 测试1 */
    //Person* person = new Person();
    //QString name = person->metaObject()->className(); // "Person"
    //QString name2 = Person::staticMetaObject.className(); // "Person"

    /* SqliteHelper */
    //DB::SqliteHelper sqliteHelper;
    //sqliteHelper.myTest();

    /* HttpSever */
    //HttpServer httpServer(8080);
    //httpServer.start();

    /* EncipherTest */
    //EncipherTest encipherTest;
    //encipherTest.monocypherTest();
    //encipherTest.Base64Test();
    //encipherTest.rsaTest();
#endif

    /* 默认创建 */
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    // 注册自定义类
    qmlRegisterType<DataEntryModel>("MyDataEntryModel",1,0,"DataEntryModel");

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

    return app.exec();
}


