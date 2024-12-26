#ifndef ELOGGERWRAPPER_H
#define ELOGGERWRAPPER_H

#include <cstdlib>

#include "easylogging++.h"
#define FMT_HEADER_ONLY
#include "fmt/format.h"

//spdlog兼容层
//#include <QUdpSocket>
#include <atomic>

#define LOG_UNUSE(x) (void)(x);

#define ELOG_LEVEL_INVALID (-1)
#define ELOG_LEVEL_TRACE   (0)
#define ELOG_LEVEL_DEBUG   (1)
#define ELOG_LEVEL_INFO    (2)
#define ELOG_LEVEL_WARN    (3)
#define ELOG_LEVEL_ERROR   (4)
#define ELOG_LEVEL_FATAL   (5)
#define ELOG_LEVEL_OFF     (6)

#if 0
#define EASYLOGGER_PRINT_CALL(flag, level, ...)                                             \
    do {                                                                                    \
        if (flag >= eloggerwrapper::easylogger_enable_flag.load(std::memory_order_relaxed)) \
        {                                                                                   \
            fmt::memory_buffer out;                                                         \
            format_to(out, __VA_ARGS__);                                                    \
            LOG(level) << out.data();                                                       \
        }                                                                                   \
    } while (0)
#else
#define EASYLOGGER_PRINT_CALL(flag, level, ...)                                             \
    do                                                                                      \
    {                                                                                       \
        if (flag >= eloggerwrapper::easylogger_enable_flag.load(std::memory_order_relaxed)) \
        {                                                                                   \
            try                                                                             \
            {                                                                               \
                LOG(level) << fmt::format(__VA_ARGS__);                                     \
            } catch (...)                                                                   \
            {                                                                               \
                qDebug() << ">>>>>> LOG PRINT ERROR: " << __FILE__ << ":" << __LINE__;      \
            }                                                                               \
        }                                                                                   \
    } while (0)
#endif

#define ELOG_TRACE(...)    EASYLOGGER_PRINT_CALL(ELOG_LEVEL_TRACE, TRACE, __VA_ARGS__)
#define ELOG_DEBUG(...)    EASYLOGGER_PRINT_CALL(ELOG_LEVEL_DEBUG, DEBUG, __VA_ARGS__)
#define ELOG_INFO(...)     EASYLOGGER_PRINT_CALL(ELOG_LEVEL_INFO, INFO, __VA_ARGS__)
#define ELOG_WARN(...)     EASYLOGGER_PRINT_CALL(ELOG_LEVEL_WARN, WARNING, __VA_ARGS__)
#define ELOG_ERROR(...)    EASYLOGGER_PRINT_CALL(ELOG_LEVEL_ERROR, ERROR, __VA_ARGS__)
#define ELOG_CRITICAL(...) EASYLOGGER_PRINT_CALL(ELOG_LEVEL_FATAL, FATAL, __VA_ARGS__)

class eloggerwrapper
{
public:
    static void logger_init(const std::string &apptag, const std::string &logpath = std::string());
    static void rolloutCallbackHandler(const char *filename, std::size_t size);
    static void logger_setlevel(int level);
    static void logger_set_format_with_fileinfo(bool en);
    static void logger_flush();
    static void logger_close();

    static std::atomic_int easylogger_enable_flag;
    static std::string app_flag_string;
};

#endif  // ELOGGERWRAPPER_H
