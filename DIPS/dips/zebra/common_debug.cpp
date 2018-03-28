#include "common_debug.h"

#ifdef WIN32
#include <windows.h>
#else
#include <time.h>
#endif

int64_t get_system_time_micros()
{
#ifdef _WIN32
    FILETIME ft;
    LARGE_INTEGER li;
    int64_t tt = 0;
    GetSystemTimeAsFileTime(&ft);
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;
    // 从1970年1月1日0:0:0:000到现在的微秒数(UTC时间)
    tt = (li.QuadPart - EPOCHFILETIME) /10;
    return tt;
#else
    timeval tv;
    gettimeofday(&tv, 0);
    return (int64_t)tv.tv_sec * 1000000 + (int64_t)tv.tv_usec;
#endif // _WIN32
    return 0;
}

string tranlate_utc_to_time(int64_t utc_time)
{
    string local_time;
    struct tm tm;
    char str[100];

    int micro_seconds = (utc_time/1000)%1000;
    int milli_seconds = utc_time%1000;

    time_t tick = (time_t)(utc_time/1000000);
    tm = *localtime(&tick);
    strftime(str, sizeof(str), "%Y-%m-%d %H:%M:%S", &tm);

    local_time.assign(str);

    local_time = local_time +
            "*" + std::to_string(micro_seconds) +
            "*" + std::to_string(milli_seconds);

    return local_time;
}


my_debug::my_debug(int level,
                   string color,
                   string time_info,
                   string file_info,
                   int line_info,
                   string func_info)
{
    log_level = level;
    log_color = color;
    log_time = time_info;
    log_file = file_info;
    log_line = line_info;
    log_func = func_info;
}

my_debug::~my_debug()
{

}
