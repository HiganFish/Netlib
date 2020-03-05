//
// Created by lsmg on 2/21/20.
//

#ifndef NETLIB_LOG_H
#define NETLIB_LOG_H

#include <cstring>
#include <iosfwd>
#include <mutex>

// 索引 最后一次出现 / 的位置并返回指针
// src/opepoll.cpp 返回的指针指向/opepoll.cpp +1 后为opepoll.cpp
#define FILENAME(x) (strrchr(x, '/')?strrchr(x, '/')+1:x)

#define LOG_ERROR(...) Net::Log::WriteLog(Net::LOG_LEVEL_ERROR, (unsigned char *)FILENAME(__FILE__), (unsigned char *)__FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...) Net::Log::WriteLog(Net::LOG_LEVEL_WARN, (unsigned char *)FILENAME(__FILE__), (unsigned char *)__FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_DEBUG(...) Net::Log::WriteLog(Net::LOG_LEVEL_DEBUG, (unsigned char *)FILENAME(__FILE__), (unsigned char *)__FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...) Net::Log::WriteLog(Net::LOG_LEVEL_INFO, (unsigned char *)FILENAME(__FILE__), (unsigned char *)__FUNCTION__, __LINE__, __VA_ARGS__)
#define ERROR_IF(r, ...)  \
{   \
if (r)  \
{   \
    LOG_ERROR(__VA_ARGS__); \
}   \
else    \
{   \
    LOG_INFO(__VA_ARGS__);  \
}   \
}

namespace Net
{
enum LogLevel
{
    LOG_LEVEL_NONE,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO
};

enum LogPlace
{
    OUT_NONE,
    OUT_CONSOLE,
    OUT_FILE
};

class Log
{
public:

    Log();

    ~Log();

    static void WriteLog(LogLevel level, unsigned char *filename, unsigned char *funcname,
                         int line, const char *format, ...);

    // 第一个参数 输出位置 第二个参数 日志打印最低等级
    static void SetLogger(LogPlace newplace, LogLevel newlevel);

private:


    void Init(LogPlace place, LogLevel level);

    void CreateLogTxt();

    static void OutputLog(LogLevel level);

    static int PrintfToBuffer(char *buffer, int size, const char *format, ...);

    static LogLevel GetLogLevel();

    static LogPlace GetLogPlace();

    static LogLevel loglevel_;
    // 日志输出位置
    static LogPlace logplace_;

    static std::mutex mutex;

    static Log log_;

    static std::string stringbuf;

    static int fd;

    Log(const Log&);
    Log& operator=(const Log&);
};

}
#endif //NETLIB_LOG_H