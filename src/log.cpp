//
// Created by lsmg on 2/21/20.
//

#include <fcntl.h>
#include <unistd.h>
#include <cstdarg>
#include "log.h"

Net::Log Net::Log::log_;
std::mutex Net::Log::mutex;
std::string Net::Log::stringbuf = "";
int Net::Log::fd = -1;

Net::LogLevel Net::Log::loglevel_ = LOG_LEVEL_NONE;
Net::LogPlace Net::Log::logplace_ = OUT_NONE;

Net::Log::Log()
{
    Init(OUT_NONE, LOG_LEVEL_NONE);
}

Net::Log::~Log()
{

}

void Net::Log::SetLogger(LogPlace newplace, LogLevel newlevel)
{
    loglevel_ = newlevel;
    logplace_ = newplace;
}

void Net::Log::Init(LogPlace place, LogLevel level)
{
    logplace_ = place;
    loglevel_ = level;

    CreateLogTxt();
}

void Net::Log::CreateLogTxt()
{
    // 当前路径
    char buffer[1024]{};
    getcwd(buffer, sizeof(buffer));

    // 获取当前时间
    time_t now = time(nullptr);
    char temp[64];
    // 格式化 20200101-20:40:20
    strftime(temp, sizeof(temp), "%Y%m%d-%H:%M:%S", localtime(&now));

    strcat(buffer, "/");
    strcat(buffer, temp);
    strcat(buffer, ".log");

    fd = open(buffer, O_CREAT | O_WRONLY | O_APPEND, 0644);
}

void Net::Log::WriteLog(LogLevel level, unsigned char* filename, unsigned char* funcname,
                        int line, const char *format, ...)
{
    // 每次输出前清空
    stringbuf.clear();

    // 获取当前时间
    time_t now = time(nullptr);
    char temp[64];
    // 格式化 20200101-20:40:20
    strftime(temp, sizeof(temp), "[%Y%m%d-%H:%M:%S]", localtime(&now));
    stringbuf.append(temp);

    if (level > Net::Log::GetLogLevel())
    {
        return;
    }
    std::string out_level;
    switch (level)
    {
        case LOG_LEVEL_NONE:
            return;
        case LOG_LEVEL_ERROR:
            out_level = "ERROR";
            break;
        case LOG_LEVEL_WARN:
            out_level = "WARN";
            break;
        case LOG_LEVEL_DEBUG:
            out_level = "DEBUG";
            break;
        case LOG_LEVEL_INFO:
            out_level = "INFO";
            break;
    }

    char locinfo[100];
    //   log级别 文件名 函数名：行号
    const char* format2 = "[%s][%-s][%s:%4d]";
    PrintfToBuffer(locinfo, sizeof(locinfo), format2,
                   out_level.c_str(), filename, funcname, line);
    stringbuf.append(locinfo);

    // 日志输出信息内容
    char infobody[256];
    va_list ap;
    va_start(ap, format);
    vsnprintf(infobody, 256, format, ap);
    va_end(ap);
    stringbuf.append(infobody);
    stringbuf.append("\n");

    OutputLog();
}

void Net::Log::OutputLog()
{
    switch (Net::Log::GetLogPlace())
    {
        case OUT_NONE:
            return;
        case OUT_CONSOLE:
            printf("%s", stringbuf.c_str());
            break;
        case OUT_FILE:
            write(fd, stringbuf.c_str(), stringbuf.length());
            break;
    }
}

Net::LogLevel Net::Log::GetLogLevel()
{
    return loglevel_;
}


Net::LogPlace Net::Log::GetLogPlace()
{
    return logplace_;
}


int Net::Log::PrintfToBuffer(char *buffer, int size, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    int ret = vsnprintf(buffer, 100, format, ap);
    va_end(ap);
    return ret;
}

