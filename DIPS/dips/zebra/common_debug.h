#ifndef COMMON_DEBUG_H
#define COMMON_DEBUG_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stdint.h>
#include <sstream>
#include <time.h>

using namespace std;



/* The switch to open or close print */
//#define MY_DEBUG_SWITCH
#define MY_DEBUG_TIME_SWITCH
#define MY_DEBUG_FILE_SWITCH



/*******************************************************************
 *using e.g:
 *MY_DEBUG(1, GREEN) << "11111111111111111" << DEBUG_END;
 *
 *
*******************************************************************/





/**************************** color define ***********************/
#define NONE          "\033[m"          //

#define GREEN         "\033[0;32;32m"   //绿色
#define LIGHT_GREEN   "\033[1;32m"

#define RED           "\033[0;32;31m"   //红色
#define LIGHT_RED     "\033[47;31m"

#define BLUE          "\033[0;32;34m"   //蓝色
#define LIGTH_BLUE    "\033[47;34m"

#define DARY_GRAY     "\033[47;30m"

#define CYAN          "\033[0;36m"      //青色
#define LIGHT_CYAN    "\033[1;36m"

#define PURPLE        "\033[0;35m"      //紫红色
#define LIGHT_PURPLE  "\033[1;35m"

#define BROWN         "\033[0;33m"      //黄青色
#define YELLOW        "\033[1;33m"      //黄色
#define LIGHT_GRAY    "\033[0;37m"      //灰白色
#define WHITE         "\033[1;37m"      //白色
#define CLEAN         "\033[2J"         //
/**************************** color define ***********************/


// 从1601年1月1日0:0:0:000到1970年1月1日0:0:0:000的时间(单位100ns)
#define EPOCHFILETIME   (116444736000000000UL)

int64_t get_system_time_micros();
string tranlate_utc_to_time(int64_t utc_time);

class my_debug
{
public:
    my_debug(int level,
             string color,
             string time_info = "",
             string file_info = "",
             int line_info = 0,
             string func_info = "");
    ~my_debug();

    ostringstream& null_stream()
    {
        return m_out;
    }

    int log_level;
    string log_color;
    string log_time;
    string log_file;
    int log_line;
    string log_func;
    ostringstream m_out;

private:

};





#ifdef MY_DEBUG_SWITCH

inline std::ostream& operator<<(std::ostream& out, my_debug& obj)
{
    out << obj.log_color.c_str();

#ifdef MY_DEBUG_TIME_SWITCH
    if(!obj.log_time.empty())
    {
        out << "time : " << obj.log_time;
        out << "\n";
    }
#endif

#ifdef MY_DEBUG_FILE_SWITCH
    if(!obj.log_file.empty())
    {
         out << "file : " << obj.log_file.c_str() << " at line(" << obj.log_line << ")";
         out << "\n";
    }
#endif

    if(!obj.log_func.empty())
    {
        out << "func : " << obj.log_func.c_str() << "()\n";
    }

    if(obj.log_level == 1)
    {
        out << "[INFO]";
    }
    else if(obj.log_level == 2)
    {
        out << "[WARN]";
    }
    else if(obj.log_level == 3)
    {
        out << "[WRONG]";
    }
    else if(obj.log_level == 4)
    {
        out << "[FAULT]";
    }
    else
    {
        out << "[NONE]";
    }

    return out;
}

#define MY_DEBUG(level, color) \
    std::cout << my_debug((level), (color), tranlate_utc_to_time(get_system_time_micros()), __FILE__, __LINE__, __FUNCTION__)

#define DEBUG_END  std::endl;std::cout<<NONE<<std::endl;

#else

inline ostringstream& operator<<(ostringstream& out, my_debug& obj)
{
    return out;
}

#define MY_DEBUG(level, color) \
    my_debug((level), (color)).null_stream()\

#define DEBUG_END  "";

#endif

#endif // COMMON_DEBUG_H
