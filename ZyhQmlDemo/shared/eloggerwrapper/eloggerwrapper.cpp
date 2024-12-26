#include "eloggerwrapper.h"

#include <QtGlobal>

#include "helptools.h"
INITIALIZE_EASYLOGGINGPP

std::atomic_int eloggerwrapper::easylogger_enable_flag = ATOMIC_VAR_INIT(ELOG_LEVEL_TRACE);
std::string eloggerwrapper::app_flag_string = std::string("None");

void eloggerwrapper::rolloutCallbackHandler(const char *filename, std::size_t size)
{
    int status = 0;
    LOG_UNUSE(status)
    LOG_UNUSE(size)

    //备份日志
    std::string bkfilename = std::string(filename) + std::string(".bak");
    ::rename(filename, bkfilename.c_str());
}

//0=trace 1=debug 2=info 3=warn 4=err 5=critical 6=off
void eloggerwrapper::logger_setlevel(int level)
{
    ELOG_WARN("更改日志等级={:d}", level);
#if 0
    int logger_level = level;
    DBGLINE();
    VLOG(1) << "Refer Log Level: 0=trace 1=debug 2=info 3=warn 4=err 5=critical 6=off, update log level to " << level;
    DBGLINE();
    el::Configurations defaultConf;
    defaultConf.parseFromText(el::Loggers::defaultConfigurations()->configurationFile());
    el::Level loglevels[] = {el::Level::Trace, el::Level::Debug, el::Level::Info, el::Level::Error, el::Level::Fatal};
    unsigned char logmasks[] = {0x3F, 0x3E, 0x3C, 0x38, 0x30, 0x20, 0x0};
    DBGLINE();
    unsigned char logmask = logmasks[level];

    for (int ii = 0; ii < 6; ii++)
    {
        if (logmask & (0x1 << ii))
        {
            defaultConf.set(loglevels[ii], el::ConfigurationType::Enabled, "true");
        }
        else
        {
            defaultConf.set(loglevels[ii], el::ConfigurationType::Enabled, "false");
        }
    }

    DBGLINE();
    if (logmask == 0)
    {
        //关闭日志
        DBGLINE();
        defaultConf.setGlobally(el::ConfigurationType::Enabled, "false");
    }
    else
    {
        //启用日志
        DBGLINE();
        defaultConf.setGlobally(el::ConfigurationType::Enabled, "true");
    }
    DBGLINE();

    DBGLINE();
    el::Loggers::reconfigureLogger("default", defaultConf);
    // Actually reconfigure all loggers instead
    el::Loggers::reconfigureAllLoggers(defaultConf);
    if (logger_level != ELOG_LEVEL_INVALID)
    {
        easylogger_enable_flag.store(logger_level);
    }
    DBGLINE();
#else
    int logger_level = level;
    el::Configurations defaultConf;
    defaultConf.parseFromText(el::Loggers::defaultConfigurations()->configurationFile());

    VLOG(1) << "Refer Log Level: 0=trace 1=debug 2=info 3=warn 4=err 5=critical 6=off";

    switch (level)
    {
        case 0:  //trace
            defaultConf.setGlobally(el::ConfigurationType::Enabled, "true");
            defaultConf.set(el::Level::Trace, el::ConfigurationType::Enabled, "true");
            defaultConf.set(el::Level::Debug, el::ConfigurationType::Enabled, "true");
            defaultConf.set(el::Level::Info, el::ConfigurationType::Enabled, "true");
            defaultConf.set(el::Level::Warning, el::ConfigurationType::Enabled, "true");
            defaultConf.set(el::Level::Error, el::ConfigurationType::Enabled, "true");
            defaultConf.set(el::Level::Fatal, el::ConfigurationType::Enabled, "true");
            break;
        case 1:  //debug
            defaultConf.setGlobally(el::ConfigurationType::Enabled, "true");
            defaultConf.set(el::Level::Trace, el::ConfigurationType::Enabled, "false");
            defaultConf.set(el::Level::Debug, el::ConfigurationType::Enabled, "true");
            defaultConf.set(el::Level::Info, el::ConfigurationType::Enabled, "true");
            defaultConf.set(el::Level::Warning, el::ConfigurationType::Enabled, "true");
            defaultConf.set(el::Level::Error, el::ConfigurationType::Enabled, "true");
            defaultConf.set(el::Level::Fatal, el::ConfigurationType::Enabled, "true");
            break;
        case 2:  //info
            defaultConf.setGlobally(el::ConfigurationType::Enabled, "true");
            defaultConf.set(el::Level::Trace, el::ConfigurationType::Enabled, "false");
            defaultConf.set(el::Level::Debug, el::ConfigurationType::Enabled, "false");
            defaultConf.set(el::Level::Info, el::ConfigurationType::Enabled, "true");
            defaultConf.set(el::Level::Warning, el::ConfigurationType::Enabled, "true");
            defaultConf.set(el::Level::Error, el::ConfigurationType::Enabled, "true");
            defaultConf.set(el::Level::Fatal, el::ConfigurationType::Enabled, "true");
            break;
        case 3:  //warn
            defaultConf.setGlobally(el::ConfigurationType::Enabled, "true");
            defaultConf.set(el::Level::Trace, el::ConfigurationType::Enabled, "false");
            defaultConf.set(el::Level::Debug, el::ConfigurationType::Enabled, "false");
            defaultConf.set(el::Level::Info, el::ConfigurationType::Enabled, "false");
            defaultConf.set(el::Level::Warning, el::ConfigurationType::Enabled, "true");
            defaultConf.set(el::Level::Error, el::ConfigurationType::Enabled, "true");
            defaultConf.set(el::Level::Fatal, el::ConfigurationType::Enabled, "true");
            break;
        case 4:  //err
            defaultConf.setGlobally(el::ConfigurationType::Enabled, "true");
            defaultConf.set(el::Level::Trace, el::ConfigurationType::Enabled, "false");
            defaultConf.set(el::Level::Debug, el::ConfigurationType::Enabled, "false");
            defaultConf.set(el::Level::Info, el::ConfigurationType::Enabled, "false");
            defaultConf.set(el::Level::Warning, el::ConfigurationType::Enabled, "false");
            defaultConf.set(el::Level::Error, el::ConfigurationType::Enabled, "true");
            defaultConf.set(el::Level::Fatal, el::ConfigurationType::Enabled, "true");
            break;
        case 5:  //critial
            defaultConf.setGlobally(el::ConfigurationType::Enabled, "true");
            defaultConf.set(el::Level::Trace, el::ConfigurationType::Enabled, "false");
            defaultConf.set(el::Level::Debug, el::ConfigurationType::Enabled, "false");
            defaultConf.set(el::Level::Info, el::ConfigurationType::Enabled, "false");
            defaultConf.set(el::Level::Warning, el::ConfigurationType::Enabled, "false");
            defaultConf.set(el::Level::Error, el::ConfigurationType::Enabled, "false");
            defaultConf.set(el::Level::Fatal, el::ConfigurationType::Enabled, "true");
            break;
        case 6:  //off
            defaultConf.setGlobally(el::ConfigurationType::Enabled, "false");
            break;
        default:
            logger_level = ELOG_LEVEL_INVALID;
            VLOG(1) << "error log level, please refer:0=trace 1=debug 2=info 3=warn 4=err 5=critical 6=off";
            break;
    }

    el::Loggers::reconfigureLogger("default", defaultConf);
    // Actually reconfigure all loggers instead
    el::Loggers::reconfigureAllLoggers(defaultConf);
    if (logger_level != ELOG_LEVEL_INVALID)
    {
        easylogger_enable_flag.store(logger_level);
    }
#endif
}

void eloggerwrapper::logger_set_format_with_fileinfo(bool en)
{
    el::Configurations defaultConf;
    defaultConf.parseFromText(el::Loggers::defaultConfigurations()->configurationFile());

    if (en)
    {
        defaultConf.setGlobally(el::ConfigurationType::Format, std::string("[") + app_flag_string + std::string("] ") + std::string("%datetime{%Y-%M-%d %H:%m:%s} <%levshort> [%fbase:%line] %msg"));
    }
    else
    {
        defaultConf.setGlobally(el::ConfigurationType::Format, std::string("[") + app_flag_string + std::string("] ") + std::string("%datetime{%Y-%M-%d %H:%m:%s} <%levshort> %msg"));
    }

    el::Loggers::reconfigureLogger("default", defaultConf);
    // Actually reconfigure all loggers instead
    el::Loggers::reconfigureAllLoggers(defaultConf);
}

void eloggerwrapper::logger_flush()
{
    el::Loggers::flushAll();
}

void eloggerwrapper::logger_close()
{
    ;
}

//tag: log name: fbkend,playerbox
void eloggerwrapper::logger_init(const std::string &apptag, const std::string &logpath)
{
    el::Configurations defaultConf;
    defaultConf.setToDefault();

    //    //安装udp网络日志
    //    el::Helpers::installLogDispatchCallback<NetworkDispatcher>("NetworkDispatcher");
    //    // you can uninstall default one by
    //    // el::Helpers::uninstallLogDispatchCallback<el::base::DefaultLogDispatchCallback>("DefaultLogDispatchCallback");
    //    // Set server params
    //    NetworkDispatcher* dispatcher = el::Helpers::logDispatchCallback<NetworkDispatcher>("NetworkDispatcher");
    //    dispatcher->setEnabled(true);

    app_flag_string = apptag;

    //------------

    //el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);
    //el::Loggers::addFlag(el::LoggingFlag::HierarchicalLogging);
    el::Loggers::setLoggingLevel(el::Level::Info);

    // To set GLOBAL configurations you may use; Values are always std::string
    defaultConf.setGlobally(el::ConfigurationType::Enabled, "true");

    //#ifdef Q_OS_WIN     //easylogingpp的标准输出功能在windows下QtCreator程序输出窗口中文乱码，使用qWaring替代
    defaultConf.setGlobally(el::ConfigurationType::ToStandardOutput, "false");
    //#else
    //   defaultConf.setGlobally(el::ConfigurationType::ToStandardOutput, "true");
    //#endif

    //defaultConf.setGlobally(el::ConfigurationType::LogFlushThreshold, "1");

    if (logpath.empty())
    {
        //日志不保存到文件
        defaultConf.setGlobally(el::ConfigurationType::ToFile, "false");
    }
    else
    {
        defaultConf.setGlobally(el::ConfigurationType::ToFile, "true");
        defaultConf.setGlobally(el::ConfigurationType::Filename, logpath);
        //设置日志文件大小:滚动日志
        defaultConf.setGlobally(el::ConfigurationType::MaxLogFileSize, std::to_string(1024 * 1024 * 3));
        //滚动日志时备份日志
        el::Helpers::installPreRollOutCallback(eloggerwrapper::rolloutCallbackHandler);
        //日志滚动检查策略
        el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
    }

#ifdef __ARM_EABI__
    defaultConf.setGlobally(el::ConfigurationType::Format, std::string("[") + app_flag_string + std::string("] ") + std::string("%datetime{%Y-%M-%d %H:%m:%s} <%levshort> %msg"));
#else
    defaultConf.setGlobally(el::ConfigurationType::Format, std::string("[") + app_flag_string + std::string("] ") + std::string("%datetime{%Y-%M-%d %H:%m:%s.%z} <%levshort> [%fbase:%line] %msg"));

#endif
    el::Loggers::reconfigureLogger("default", defaultConf);
    // Actually reconfigure all loggers instead
    el::Loggers::reconfigureAllLoggers(defaultConf);
    el::Loggers::setVerboseLevel(1);
    DBGLINE();
    logger_setlevel(2);
    DBGLINE();
}
