#include "httpserver.h"

#include <QDateTime>
#include <chrono>
#include <exception>
#include <future>
#include <thread>

#include "helptools.h"
#include "messagebroker/publisher.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace httplib;

//HTTP响应码
enum ENUM_HTTP_REPONSE_CODE {
    HTTP_OK = 200,           /*请求成功*/
    HTTP_BAD_REQUEST = 400,  /*错误请求*/
    HTTP_UNAUTHORIZED = 401, /*请求要求用户的身份认证*/
    HTTP_FORBIDDEN = 403,    /*客户端没有访问内容的权限*/
    HTTP_NOT_FOUND = 404,    /*服务器找不到请求的资源*/
    HTTP_SERVER_ERROR = 500  /*服务器内部错误*/
};

//消息错误码
enum ENUM_HTTP_ERROR_CODE {
    ERROR_OK = 0,                   /*无错误*/
    ERROR_PARAM_EMPTY = 10001,      /*参数为空*/
    ERROR_PARAM_INCOMPLETE = 10002, /*参数不全*/
    ERROR_PARAM_TYPE_INVALID,       /*参数类型错误*/
    ERROR_PARAM_INVALID,            /*参数无效*/
    ERROR_DEVICE_FAULT = 20001,     /*设备内部错误*/
};

HttpServer::HttpServer(int port)
    : m_port(port)
{
    m_httpserver = new httplib::Server;

    m_myserialport = new MySerialPort(false);
    m_myserialport->openPort("/dev/ttyv0");
    long sleepMs = 200;
    m_httpserver->Post("/v2/ic/read_id", [this, sleepMs](const Request &req, Response &res) {
        Q_UNUSED(req)
        Q_UNUSED(res)
        int errcode = ERROR_OK;
        std::string errmsg("读卡号成功");

        QByteArray hexArray;
        try
        {
            //json jbody = json::parse(req.body);
            //QString dev_id = QString::fromStdString(jbody["dev_id"].get<std::string>());
            //qDebug() << dev_id;

            if (this->m_myserialport->isOpen()) {
                this->m_myserialport->cmdReadId();
                QThread::msleep(sleepMs);
                hexArray = this->m_myserialport->readMessage();

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
            }
        } catch (std::exception &e)
        {
            qDebug() << "[HttpServer] 解析请求参数失败:{:s}", e.what();
            errcode = ERROR_PARAM_INCOMPLETE;
            errmsg = std::string("请求参数不全");
        }

        json retj;
        retj["dev_id"] = "1";
        retj["errcode"] = errcode;
        retj["errmsg"] = errmsg;
        retj["timestamp"] = HPT::get_current_isodate_timestamp().toStdString();
        retj["hex_array"] = hexArray;

        res.set_content(retj.dump(), "application/json; charset=utf-8");

        res.status = 200;
    });
    m_httpserver->Post("/v2/ic/read_block", [this, sleepMs](const Request &req, Response &res) {
        Q_UNUSED(req)
        Q_UNUSED(res)
        int errcode = ERROR_OK;
        std::string errmsg("读指定块数据成功");

        QByteArray hexArray;
        try
        {
            if (this->m_myserialport->isOpen()) {
                this->m_myserialport->cmdReadBlockHex();
                QThread::msleep(sleepMs);
                hexArray = this->m_myserialport->readMessage();

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
            }
        } catch (std::exception &e)
        {
            qDebug() << "[HttpServer] 解析请求参数失败:{:s}", e.what();
            errcode = ERROR_PARAM_INCOMPLETE;
            errmsg = std::string("请求参数不全");
        }

        json retj;
        retj["dev_id"] = "1";
        retj["errcode"] = errcode;
        retj["errmsg"] = errmsg;
        retj["timestamp"] = HPT::get_current_isodate_timestamp().toStdString();
        retj["hex_array"] = hexArray;

        res.set_content(retj.dump(), "application/json; charset=utf-8");

        res.status = 200;
    });
    m_httpserver->Post("/v2/ic/write_block", [this, sleepMs](const Request &req, Response &res) {
        Q_UNUSED(req)
        Q_UNUSED(res)
        int errcode = ERROR_OK;
        std::string errmsg("写数据到指定块成功");

        QByteArray hexArray;
        try
        {
            if (this->m_myserialport->isOpen()) {
                this->m_myserialport->cmdWriteBlockHex();
                QThread::msleep(sleepMs);
                hexArray = this->m_myserialport->readMessage();

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
            }
        } catch (std::exception &e)
        {
            qDebug() << "[HttpServer] 解析请求参数失败:{:s}", e.what();
            errcode = ERROR_PARAM_INCOMPLETE;
            errmsg = std::string("请求参数不全");
        }

        json retj;
        retj["dev_id"] = "1";
        retj["errcode"] = errcode;
        retj["errmsg"] = errmsg;
        retj["timestamp"] = HPT::get_current_isodate_timestamp().toStdString();
        retj["hex_array"] = hexArray;

        res.set_content(retj.dump(), "application/json; charset=utf-8");

        res.status = 200;
    });
    m_httpserver->Post("/v2/ic/init_card", [this, sleepMs](const Request &req, Response &res) {
        Q_UNUSED(req)
        Q_UNUSED(res)
        int errcode = ERROR_OK;
        std::string errmsg("初始化钱包成功");

        QByteArray hexArray;
        try
        {
            if (this->m_myserialport->isOpen()) {
                this->m_myserialport->cmdInitCard();
                QThread::msleep(sleepMs);
                hexArray = this->m_myserialport->readMessage();

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
            }
        } catch (std::exception &e)
        {
            qDebug() << "[HttpServer] 解析请求参数失败:{:s}", e.what();
            errcode = ERROR_PARAM_INCOMPLETE;
            errmsg = std::string("请求参数不全");
        }

        json retj;
        retj["dev_id"] = "1";
        retj["errcode"] = errcode;
        retj["errmsg"] = errmsg;
        retj["timestamp"] = HPT::get_current_isodate_timestamp().toStdString();
        retj["hex_array"] = hexArray;

        res.set_content(retj.dump(), "application/json; charset=utf-8");

        res.status = 200;
    });
    m_httpserver->Post("/v2/ic/read_card", [this, sleepMs](const Request &req, Response &res) {
        Q_UNUSED(req)
        Q_UNUSED(res)
        int errcode = ERROR_OK;
        std::string errmsg("钱包值（余额）查询成功");

        QByteArray hexArray;
        try
        {
            if (this->m_myserialport->isOpen()) {
                this->m_myserialport->cmdReadCard();
                QThread::msleep(sleepMs);
                hexArray = this->m_myserialport->readMessage();

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
            }
        } catch (std::exception &e)
        {
            qDebug() << "[HttpServer] 解析请求参数失败:{:s}", e.what();
            errcode = ERROR_PARAM_INCOMPLETE;
            errmsg = std::string("请求参数不全");
        }

        json retj;
        retj["dev_id"] = "1";
        retj["errcode"] = errcode;
        retj["errmsg"] = errmsg;
        retj["timestamp"] = HPT::get_current_isodate_timestamp().toStdString();
        retj["hex_array"] = hexArray;

        res.set_content(retj.dump(), "application/json; charset=utf-8");

        res.status = 200;
    });
    m_httpserver->Post("/v2/ic/card_add", [this, sleepMs](const Request &req, Response &res) {
        Q_UNUSED(req)
        Q_UNUSED(res)
        int errcode = ERROR_OK;
        std::string errmsg("钱包充值（加值）成功");

        QByteArray hexArray;
        try
        {
            if (this->m_myserialport->isOpen()) {
                this->m_myserialport->cmdCardAdd();
                QThread::msleep(sleepMs);
                hexArray = this->m_myserialport->readMessage();

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
            }
        } catch (std::exception &e)
        {
            qDebug() << "[HttpServer] 解析请求参数失败:{:s}", e.what();
            errcode = ERROR_PARAM_INCOMPLETE;
            errmsg = std::string("请求参数不全");
        }

        json retj;
        retj["dev_id"] = "1";
        retj["errcode"] = errcode;
        retj["errmsg"] = errmsg;
        retj["timestamp"] = HPT::get_current_isodate_timestamp().toStdString();
        retj["hex_array"] = hexArray;

        res.set_content(retj.dump(), "application/json; charset=utf-8");

        res.status = 200;
    });
    m_httpserver->Post("/v2/ic/card_del", [this, sleepMs](const Request &req, Response &res) {
        Q_UNUSED(req)
        Q_UNUSED(res)
        int errcode = ERROR_OK;
        std::string errmsg("钱包扣款（减值）成功");

        QByteArray hexArray;
        try
        {
            if (this->m_myserialport->isOpen()) {
                this->m_myserialport->cmdCardDel();
                QThread::msleep(sleepMs);
                hexArray = this->m_myserialport->readMessage();

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
            }
        } catch (std::exception &e)
        {
            qDebug() << "[HttpServer] 解析请求参数失败:{:s}", e.what();
            errcode = ERROR_PARAM_INCOMPLETE;
            errmsg = std::string("请求参数不全");
        }

        json retj;
        retj["dev_id"] = "1";
        retj["errcode"] = errcode;
        retj["errmsg"] = errmsg;
        retj["timestamp"] = HPT::get_current_isodate_timestamp().toStdString();
        retj["hex_array"] = hexArray;

        res.set_content(retj.dump(), "application/json; charset=utf-8");

        res.status = 200;
    });

    //配置接口
    m_httpserver->Post("/actions/updateApp", [](const Request &req, Response &res) {
        //        int status = 200;
        int errcode = 0;
        std::string errmsg("成功");

        try
        {
            qDebug() << "[HttpServer] 接收App升级命令";
            auto image_file = req.get_file_value("appfile");

            if (image_file.content.length())
            {
                qDebug() << "[HttpServer] image length > 0";
            } else {
                qDebug() << "[HttpServer] image length <= 0";
            }

        } catch (std::exception &e)
        {
            qDebug() << "[HttpServer] 执行App升级失败:{:s}", e.what();
            errcode = 4;
            errmsg = std::string("请求参数缺失[appfile]字段");
        }

        json retj;
        retj["errcode"] = errcode;
        retj["errmsg"] = errmsg;
        retj["timestamp"] = QDateTime::currentDateTimeUtc().toString(Qt::ISODate).toStdString();

        res.set_content(retj.dump(), "application/json; charset=utf-8");
        if (errcode != 0)
        {
            res.status = 412;
            qDebug() << "[HttpServer] {:s}", errmsg;
        }
        else
            res.status = 200;

        if (errcode == 0)
        {
            qDebug() << "[HttpServer] 开始App升级...";
        }
    });

    //深圳: 攸亮一体化综合智能机柜通信协议v1.2
    //查询前端设备基础信息
    m_httpserver->Get("/v1/management/device_base_info", [](const Request &req, Response &res) {
        Q_UNUSED(req)
        json retj;
        int errcode = ERROR_OK;
        std::string errmsg("成功");

        qDebug() << "[HttpServer] 接收查询前端设备基础信息命令";
        try
        {
            retj["data"]["dev_id"] = "1";
            retj["data"]["location_code"] = "2";
            retj["data"]["sn"] = "3";
            retj["data"]["mac"] = "4";
            retj["data"]["brand"] = "5";
            retj["data"]["model"] = "6";
            retj["data"]["version"] = "7";
            retj["data"]["productive_time"] = "8";

        } catch (std::exception &e)
        {
            qDebug() << "[HttpServer] 解析请求参数失败:{:s}", e.what();
            errcode = ERROR_DEVICE_FAULT;
            errmsg = std::string("响应参数填充失败");
        }

        retj["dev_id"] = "1";
        retj["errcode"] = errcode;
        retj["errmsg"] = errmsg;
        retj["timestamp"] = HPT::get_current_isodate_timestamp().toStdString();

        res.set_content(retj.dump(), "application/json; charset=utf-8");

        if (errcode != ERROR_OK)
        {
            qDebug() << "[HttpServer] {:s}", errmsg;
            res.status = HTTP_SERVER_ERROR;
        }
        else
            res.status = HTTP_OK;
    });

    //配置设备网络参数
    m_httpserver->Post("/v1/management/device_network_params", [](const Request &req, Response &res) {
        int errcode = ERROR_OK;
        std::string errmsg("成功");

        try
        {
            qDebug() << "[HttpServer] 接收配置设备网络参数命令";
            json jbody = json::parse(req.body);

            QString ip = QString::fromStdString(jbody["ip"].get<std::string>());
            QString netmask = QString::fromStdString(jbody["netmask"].get<std::string>());
            QString gateway = QString::fromStdString(jbody["gateway"].get<std::string>());
            int local_listen_port = jbody["local_listen_port"].get<int>();
            QString alarm_ip = QString::fromStdString(jbody["alarm_ip"].get<std::string>());
            int alarm_port = jbody["alarm_port"].get<int>();

            if (!HPT::check_normal_ip(ip))
            {
                errcode = ERROR_PARAM_INVALID;
                throw std::invalid_argument("设备IP地址无效");
            }

            if (!HPT::check_normal_ip(alarm_ip))
            {
                errcode = ERROR_PARAM_INVALID;
                throw std::invalid_argument("告警主机IP地址无效");
            }

            if (!HPT::check_normal_ip(gateway))
            {
                errcode = ERROR_PARAM_INVALID;
                throw std::invalid_argument("网关地址无效");
            }

            if (!HPT::check_normal_netmask(netmask))
            {
                errcode = ERROR_PARAM_INVALID;
                throw std::invalid_argument("子网掩码无效");
            }

            if ((local_listen_port < 10000) || (local_listen_port > 30000))
            {
                errcode = ERROR_PARAM_INVALID;
                throw std::range_error("设备监听端口超出有效范围[10000,30000]");
            }

            if ((alarm_port < 10000) || (alarm_port > 30000))
            {
                errcode = ERROR_PARAM_INVALID;
                throw std::range_error("告警主机端口超出有效范围[10000,30000]");
            }

            //保持配置
            qDebug() << ip << netmask << gateway << local_listen_port << alarm_ip << alarm_port;
        } catch (std::exception &e)
        {
            qDebug() << "[HttpServer] 解析请求参数失败:{:s}", e.what();
            if (errcode == ERROR_OK)
            {
                errcode = ERROR_PARAM_INCOMPLETE;
            }

            errmsg = std::string("解析参数失败:") + std::string(e.what());
        }

        json retj;
        retj["dev_id"] = "1";
        retj["errcode"] = errcode;
        retj["errmsg"] = errmsg;
        retj["timestamp"] = HPT::get_current_isodate_timestamp().toStdString();

        res.set_content(retj.dump(), "application/json; charset=utf-8");

        if (errcode != ERROR_OK)
        {
            qDebug() << "[HttpServer] {:s}", errmsg;
            res.status = HTTP_BAD_REQUEST;
        }
        else
            res.status = HTTP_OK;
    });

    //配置前端设备点位编号
    m_httpserver->Post("/v1/management/location_code", [](const Request &req, Response &res) {
        int errcode = ERROR_OK;
        std::string errmsg("成功");

        try
        {
            qDebug() << "[HttpServer] 接收配置前端设备点位编号命令";
            json jbody = json::parse(req.body);

            QString location_code = QString::fromStdString(jbody["location_code"].get<std::string>());

            //保持配置
            qDebug() << location_code;
        } catch (std::exception &e)
        {
            qDebug() << "[HttpServer] 解析请求参数失败:{:s}", e.what();
            errcode = ERROR_PARAM_INCOMPLETE;
            errmsg = std::string("请求参数不全");
        }

        json retj;
        retj["dev_id"] = "1";
        retj["errcode"] = errcode;
        retj["errmsg"] = errmsg;
        retj["timestamp"] = HPT::get_current_isodate_timestamp().toStdString();

        res.set_content(retj.dump(), "application/json; charset=utf-8");

        if (errcode != ERROR_OK)
        {
            qDebug() << "[HttpServer] {:s}", errmsg;
            res.status = HTTP_BAD_REQUEST;
        }
        else
            res.status = HTTP_OK;
    });

    //配置前端设备编号
    m_httpserver->Post("/v1/management/dev_id", [](const Request &req, Response &res) {
        int errcode = ERROR_OK;
        std::string errmsg("成功");

        try
        {
            qDebug() << "[HttpServer] 接收配置前端设备编号命令";
            json jbody = json::parse(req.body);

            QString dev_id = QString::fromStdString(jbody["dev_id"].get<std::string>());

            //保持配置
            qDebug() << dev_id;
        } catch (std::exception &e)
        {
            qDebug() << "[HttpServer] 解析请求参数失败:{:s}", e.what();
            errcode = ERROR_PARAM_INCOMPLETE;
            errmsg = std::string("请求参数不全");
        }

        json retj;
        retj["dev_id"] = "1";
        retj["errcode"] = errcode;
        retj["errmsg"] = errmsg;
        retj["timestamp"] = HPT::get_current_isodate_timestamp().toStdString();

        res.set_content(retj.dump(), "application/json; charset=utf-8");

        if (errcode != ERROR_OK)
        {
            qDebug() << "[HttpServer] {:s}", errmsg;
            res.status = HTTP_BAD_REQUEST;
        }
        else
            res.status = HTTP_OK;
    });

    //远程开锁
    m_httpserver->Post("/v1/management/open_door", [](const Request &req, Response &res) {
        int errcode = ERROR_OK;
        std::string errmsg("成功");

        try
        {
            qDebug() << "[HttpServer] 接收远程开锁命令";
            json jbody = json::parse(req.body);
            int doorindex = jbody["door_pos"].get<unsigned int>();
            qDebug() << doorindex;

        } catch (std::exception &e)
        {
            qDebug() << "[HttpServer] 解析请求参数失败:{:s}", e.what();
            errmsg = e.what();
            if (errcode == ERROR_OK)
            {
                errcode = ERROR_PARAM_INCOMPLETE;
            }
        }

        json retj;
        retj["dev_id"] = "1";
        retj["errcode"] = errcode;
        retj["errmsg"] = errmsg;
        retj["timestamp"] = HPT::get_current_isodate_timestamp().toStdString();

        res.set_content(retj.dump(), "application/json; charset=utf-8");

        if (errcode != ERROR_OK)
        {
            qDebug() << "[HttpServer] {:s}", errmsg;
            res.status = HTTP_NOT_FOUND;
        }
        else
            res.status = 200;
    });

    //配置IC卡只读密码
    m_httpserver->Post("/v1/management/door_passwd", [](const Request &req, Response &res) {
        int errcode = ERROR_OK;
        std::string errmsg("成功");

        try
        {
            qDebug() << "[HttpServer] 接收配置IC卡只读密码命令";
            json jbody = json::parse(req.body);
            std::string keyA_b64 = jbody["passwd"].get<std::string>();

            QByteArray _keyA_b64 = QByteArray::fromStdString(keyA_b64);
            QByteArray _keyA = QByteArray::fromBase64(_keyA_b64);
            if (_keyA.size() != 6)
            {
                throw std::invalid_argument("参数错误");
            }

            //保存配置
            qDebug() << _keyA;

        } catch (std::exception &e)
        {
            qDebug() << "[HttpServer] 解析请求参数失败:{:s}", e.what();
            errmsg = e.what();
            errcode = ERROR_PARAM_INCOMPLETE;
        }

        json retj;
        retj["dev_id"] = "1";
        retj["errcode"] = errcode;
        retj["errmsg"] = errmsg;
        retj["timestamp"] = HPT::get_current_isodate_timestamp().toStdString();

        res.set_content(retj.dump(), "application/json; charset=utf-8");

        if (errcode != ERROR_OK)
        {
            qDebug() << "[HttpServer] {:s}", errmsg;
            res.status = HTTP_BAD_REQUEST;
        }
        else
            res.status = 200;
    });

    //配置门禁开锁权限
    m_httpserver->Post("/v1/management/door_permissions", [](const Request &req, Response &res) {
        int errcode = ERROR_OK;
        std::string errmsg("成功");

        try
        {
            qDebug() << "[HttpServer] 接收配置门禁开锁权限命令";
            json jbody = json::parse(req.body);
            std::string method = jbody["method"].get<std::string>();
            QVariantMap perssion_map;

            auto do_get_permission_map = [&perssion_map, &jbody, &errcode]() {
                json jpermissions = jbody["permissions"];  //array
                for (auto &element : jpermissions)
                {
                    auto roleid = element["role_id"].get<std::string>();
                    if (roleid.size() != 16)
                    {
                        errcode = ERROR_PARAM_INVALID;
                        throw std::invalid_argument("role_id长度错误");
                    }

                    quint8 permission_mask = 0;
                    auto jpermission = element["permission"];  //object
                    auto door_xhsd = jpermission["door_xhsd"].get<int>();
                    auto door_fl = jpermission["door_fl"].get<int>();
                    auto door_fr = jpermission["door_fr"].get<int>();
                    auto door_bl = jpermission["door_bl"].get<int>();
                    auto door_br = jpermission["door_br"].get<int>();
                    auto door_power = jpermission["door_power"].get<int>();

                    if (door_fl) permission_mask |= (0x1 << 0);
                    if (door_fr) permission_mask |= (0x1 << 1);
                    if (door_bl) permission_mask |= (0x1 << 2);
                    if (door_br) permission_mask |= (0x1 << 3);
                    if (door_xhsd) permission_mask |= (0x1 << 4);
                    if (door_power) permission_mask |= (0x1 << 5);

                    perssion_map.insert(QString::fromStdString(roleid), permission_mask);
                }
            };

            //保存配置
            qDebug() << QString::fromStdString(method) << perssion_map;

            errcode = ERROR_OK;

        } catch (std::exception &e)
        {
            qDebug() << "[HttpServer] 解析请求参数失败:{:s}", e.what();
            errmsg = e.what();
            if (errcode == ERROR_OK)
            {
                errcode = ERROR_PARAM_INCOMPLETE;
            }
        }

        json retj;
        retj["dev_id"] = "1";
        retj["errcode"] = errcode;
        retj["errmsg"] = errmsg;
        retj["timestamp"] = HPT::get_current_isodate_timestamp().toStdString();

        res.set_content(retj.dump(), "application/json; charset=utf-8");

        if (errcode != ERROR_OK)
        {
            qDebug() << "[HttpServer] {:s}", errmsg;
            res.status = HTTP_BAD_REQUEST;
        }
        else
            res.status = 200;
    });

    //查询门禁开锁权限
    m_httpserver->Get("/v1/management/door_permissions", [](const Request &req, Response &res) {
        Q_UNUSED(req)
        json retj;
        int errcode = ERROR_OK;
        std::string errmsg("成功");

        qDebug() << "[HttpServer] 接收查询门禁开锁权限命令";
        try
        {
            QVariantMap rolepermissions;
            rolepermissions["1"] = "1";
            rolepermissions["2"] = "2";
            rolepermissions["3"] = "3";
            QMapIterator<QString, QVariant> iter(rolepermissions);
            auto jpermissions = json::array();
            const char *door_names[] = {"door_fl", "door_fr", "door_bl", "door_br", "door_xhsd", "door_power"};

            while (iter.hasNext())
            {
                iter.next();

                auto jsubobj = json::object();
                jsubobj["role_id"] = iter.key().toStdString();
                quint8 permisson_mask = iter.value().toUInt() & 0xff;
                for (int ii = 0; ii < 6; ii++)
                {
                    if (permisson_mask & (0x1 << ii))
                        jsubobj["permission"][door_names[ii]] = 1;
                    else
                        jsubobj["permission"][door_names[ii]] = 0;
                }

                jpermissions.push_back(jsubobj);
            }

            retj["data"]["permissions"] = jpermissions;

        } catch (std::exception &e)
        {
            qDebug() << "[HttpServer] 解析请求参数失败:{:s}", e.what();
            errcode = ERROR_DEVICE_FAULT;
            errmsg = std::string("响应参数填充失败");
        }

        retj["dev_id"] = "1";
        retj["errcode"] = errcode;
        retj["errmsg"] = errmsg;
        retj["timestamp"] = HPT::get_current_isodate_timestamp().toStdString();

        res.set_content(retj.dump(), "application/json; charset=utf-8");

        if (errcode != ERROR_OK)
        {
            qDebug() << "[HttpServer] {:s}", errmsg;
            res.status = HTTP_SERVER_ERROR;
        }
        else
            res.status = HTTP_OK;
    });

    //批量下发IC卡号
    m_httpserver->Post("/v1/management/door_card_ids", [](const Request &req, Response &res) {
        int errcode = ERROR_OK;
        std::string errmsg("成功");

        try
        {
            qDebug() << "[HttpServer] 接收批量下发IC卡号命令";
            json jbody = json::parse(req.body);
            auto jcard_ids = jbody["card_ids"];
            QStringList cardlist;

            for (auto &jcardid : jcard_ids)
            {
                std::string cardid = jcardid.get<std::string>();
                cardlist.append(QString::fromStdString(cardid));
            }

            //保存配置
            qDebug() << cardlist;
        } catch (std::exception &e)
        {
            qDebug() << "[HttpServer] 解析请求参数失败:{:s}", e.what();
            errmsg = e.what();
            errcode = ERROR_PARAM_INCOMPLETE;
        }

        json retj;
        retj["dev_id"] = "1";
        retj["errcode"] = errcode;
        retj["errmsg"] = errmsg;
        retj["timestamp"] = HPT::get_current_isodate_timestamp().toStdString();

        res.set_content(retj.dump(), "application/json; charset=utf-8");

        if (errcode != ERROR_OK)
        {
            qDebug() << "[HttpServer] {:s}", errmsg;
            res.status = HTTP_BAD_REQUEST;
        }
        else
            res.status = 200;
    });

    //配置NTP校时参数
    m_httpserver->Post("/v1/management/ntp_params", [](const Request &req, Response &res) {
        int errcode = ERROR_OK;
        std::string errmsg("成功");

        try
        {
            qDebug() << ("[HttpServer] 接收配置NTP校时参数命令");
            json jbody = json::parse(req.body);

            bool ntp_enable = jbody["enable"].get<bool>();
            std::string ntp_ip = jbody["ntp_ip"].get<std::string>();
            int ntp_port = jbody["ntp_port"].get<int>();
            int interval = jbody["interval"].get<int>();

            QString ntp_ip_str = QString::fromStdString(ntp_ip);
            if (!HPT::check_normal_ip(ntp_ip_str))
            {
                errcode = ERROR_PARAM_INVALID;
                throw std::invalid_argument("NTP服务器地址非法");
            }

            //保存配置
            //立即同步
            qDebug() << ntp_enable << QString::fromStdString(ntp_ip) << ntp_port << interval;

        } catch (std::exception &e)
        {
            qDebug() << "[HttpServer] 解析请求参数失败:{:s}", e.what();
            errmsg = e.what();
            if (errcode == ERROR_OK)
                errcode = ERROR_PARAM_INCOMPLETE;
        }

        json retj;
        retj["dev_id"] = "1";
        retj["errcode"] = errcode;
        retj["errmsg"] = errmsg;
        retj["timestamp"] = HPT::get_current_isodate_timestamp().toStdString();

        res.set_content(retj.dump(), "application/json; charset=utf-8");

        if (errcode != ERROR_OK)
        {
            qDebug() << "[HttpServer] {:s}", errmsg;
            res.status = HTTP_BAD_REQUEST;
        }
        else
            res.status = 200;
    });

    //查询NTP校时参数
    m_httpserver->Get("/v1/management/ntp_params", [](const Request &req, Response &res) {
        Q_UNUSED(req)
        json retj;
        int errcode = ERROR_OK;
        std::string errmsg("成功");

        qDebug() <<("[HttpServer] 接收查询NTP校时参数命令");
        try
        {
            retj["data"]["enable"] = true;
            retj["data"]["ntp_ip"] = "192.168.1.134";
            retj["data"]["ntp_port"] = 124;
            retj["data"]["interval"] = 1234;

        } catch (std::exception &e)
        {
            qDebug() << "[HttpServer] 解析请求参数失败:{:s}", e.what();
            errcode = ERROR_DEVICE_FAULT;
            errmsg = std::string("响应参数填充失败");
        }

        retj["dev_id"] = "1";
        retj["errcode"] = errcode;
        retj["errmsg"] = errmsg;
        retj["timestamp"] = HPT::get_current_isodate_timestamp().toStdString();

        res.set_content(retj.dump(), "application/json; charset=utf-8");

        if (errcode != ERROR_OK)
        {
            qDebug() << "[HttpServer] {:s}", errmsg;
            res.status = HTTP_SERVER_ERROR;
        }
        else
            res.status = HTTP_OK;
    });

    //手动校时
    m_httpserver->Post("/v1/management/dev_datetime", [](const Request &req, Response &res) {
        int errcode = ERROR_OK;
        std::string errmsg("成功");

        try
        {
            qDebug() <<("[HttpServer] 接收配置NTP校时参数命令");
            json jbody = json::parse(req.body);

            std::string datetime_str = jbody["datetime"].get<std::string>();
            //2018-03-28T08:30:08Z
            QDateTime dt = QDateTime::fromString(QString::fromStdString(datetime_str), QString("yyyy-MM-ddThh:mm:ss"));
            if (dt.isValid())
            {
                //通知更新qml状态栏时间
                qDebug() << dt;
            }
            else
            {
                errcode = ERROR_PARAM_INVALID;
                throw std::invalid_argument("时间格式错误");
            }

        } catch (std::exception &e)
        {
            qDebug() << "[HttpServer] 解析请求参数失败:{:s}", e.what();
            errmsg = e.what();
            if (errcode == ERROR_OK)
                errcode = ERROR_PARAM_INCOMPLETE;
        }

        json retj;
        retj["dev_id"] = "1";
        retj["errcode"] = errcode;
        retj["errmsg"] = errmsg;
        retj["timestamp"] = HPT::get_current_isodate_timestamp().toStdString();

        res.set_content(retj.dump(), "application/json; charset=utf-8");

        if (errcode != ERROR_OK)
        {
            qDebug() << "[HttpServer] {:s}", errmsg;
            res.status = HTTP_BAD_REQUEST;
        }
        else
            res.status = 200;
    });

    //配置动环信息上报周期
    m_httpserver->Post("/v1/management/dynamic_envs_report_period", [](const Request &req, Response &res) {
        int errcode = ERROR_OK;
        std::string errmsg("成功");

        try
        {
            qDebug() <<("[HttpServer] 接收配置动环信息上报周期命令");
            json jbody = json::parse(req.body);

            int upload_period = jbody["period"].get<int>();

            //保存配置
            //通知刷新页面
            qDebug() << upload_period;

        } catch (std::exception &e)
        {
            qDebug() << "[HttpServer] 解析请求参数失败:{:s}", e.what();
            errmsg = e.what();
            if (errcode == ERROR_OK)
                errcode = ERROR_PARAM_INCOMPLETE;
        }

        json retj;
        retj["dev_id"] = "1";
        retj["errcode"] = errcode;
        retj["errmsg"] = errmsg;
        retj["timestamp"] = HPT::get_current_isodate_timestamp().toStdString();

        res.set_content(retj.dump(), "application/json; charset=utf-8");

        if (errcode != ERROR_OK)
        {
            qDebug() << "[HttpServer] {:s}", errmsg;
            res.status = HTTP_BAD_REQUEST;
        }
        else
            res.status = 200;
    });

    //远程执行设备整机重启
    m_httpserver->Post("/v1/management/device_reboot", [](const Request &req, Response &res) {
        Q_UNUSED(req)
        Q_UNUSED(res)
        int errcode = ERROR_OK;
        std::string errmsg("成功");

        qDebug() <<("[HttpServer] 接收远程执行设备整机重启命令");

        std::thread reboot_task = std::thread([]() {
            //远程执行设备整机重启：控制各支路依次断电，延时一段时间，
            //主控重启（默认各支路断电），根据各支路重启前通断状态配置各支路开关


            for (int ii = 0; ii < 8; ii++)
            {
                qDebug() << "[HttpServer] 远程重启: 关闭交流通道-{:d}", ii + 1;
                std::this_thread::sleep_for(std::chrono::seconds(3));
            }

            for (int ii = 0; ii < 4; ii++)
            {
                qDebug() << "[HttpServer] 远程重启: 关闭直流通道-{:d}", ii + 1;
                std::this_thread::sleep_for(std::chrono::seconds(3));
            }

            qDebug() <<("[HttpServer] 远程重启:  整机设备重启");
        });

        reboot_task.detach();

        json retj;
        retj["dev_id"] = "1";
        retj["errcode"] = errcode;
        retj["errmsg"] = errmsg;
        retj["timestamp"] = HPT::get_current_isodate_timestamp().toStdString();

        res.set_content(retj.dump(), "application/json; charset=utf-8");

        res.status = 200;
    });
}

HttpServer::~HttpServer()
{
    stop();
}

void HttpServer::start()
{
    m_future = std::async(std::launch::async, [this]() {
        qDebug() << "[HttpServer] HttpServer服务启动,监听端口:{:d}", this->m_port;
        this->m_httpserver->listen("0.0.0.0", this->m_port);
    });
}

void HttpServer::stop()
{
    if (m_httpserver)
    {
        m_httpserver->stop();
        m_future.get();
        delete m_httpserver;
        m_httpserver = nullptr;
    }
    if (m_myserialport) {
        m_myserialport->closePort();
        delete m_myserialport;
        m_myserialport = nullptr;
    }
}
