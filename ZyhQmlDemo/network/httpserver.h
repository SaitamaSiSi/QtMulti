#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>

#include <QtGlobal>
#include "httplib.h"

#include <future>

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
};

#endif // HTTPSERVER_H
