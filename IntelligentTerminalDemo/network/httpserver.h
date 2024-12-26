#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>

#include <QtGlobal>
#include "httplib.h"

#include <future>
#include "../serialport/myserialport.h"

class HttpServer
{
public:
    HttpServer(int port);
    ~HttpServer();

    void start();
    void stop();

private:
    int m_port;
    httplib::Server *m_httpserver{nullptr};
    std::future<void> m_future;
    MySerialPort *m_myserialport{nullptr};
};

#endif // HTTPSERVER_H
