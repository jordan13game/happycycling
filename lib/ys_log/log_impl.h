#ifndef _LOG_IMPL_H_
#define _LOG_IMPL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/time.h>

#include <set>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

enum
{
	LF_FATAL = 0,
	LF_ERROR,
	LF_WARN,
	LF_INFO,
	LF_TRACE,
	LF_DEBUG,
	ALL_LOG_LEVEL
};

//! 将`日志级别'或者`日志模块'序号转换成位图
#define LOG_FLAG(x)			(1 << (x))

class log_t
{
public:
	log_t();
	~log_t();

	//! 设置日志目录
	int set_path(const char *path);

	//! 设置日志文件名(不包括路径)
	int set_filename(const char *filename);

	//! 设置单个日志文件最大行数
	int set_maxline(int maxline);

	//! 设置单个日志文件最大文件大小
	int set_maxsize(int maxsize);

	//! 设置日志级别
	int enable_log_level(unsigned int log_level_flags, bool enable_flag);

	//! 设置日志模块
	int enable_log_module(const char* module, bool enable_flag);

	//! 设置日志级别
	int set_log_level(const char *level);

	//! 设置是否写屏
	int enable_print_screen(bool enable_flag);

	//! 设置是否打印到文件
	int enable_print_file(bool enable_flag);

	int open();
	int close();

	int _logfatal(const char* module, const char* fmt, ...);
	int _logerror(const char* module, const char* fmt, ...);
	int _logwarn(const char* module, const char* fmt, ...);
	int _loginfo(const char* module, const char* fmt, ...);
	int _logtrace(const char* module, const char* fmt, ...);
	int _logdebug(const char* module, const char* fmt, ...);

private:
    int open_file_thread();

	int check_module(const char* module);

	//! 检查日志级别
	int check_log_level(int log_level);

	int format_log_head(char *log_head, const char* module, int level, struct tm *tmp);

	//! 记录一行日志
	int log(const char* module, int level, const char* fmt, va_list ap, const char* color_begin_ = "", const char* color_end_= "");

	void handle_print_screen(const std::string& mesg);

	void handle_print_file(const std::string& text);

private:

  boost::asio::io_service work_io_service_;

  boost::scoped_ptr<boost::asio::io_service::work> work_;

  boost::scoped_ptr<boost::thread> work_thread_;

  std::ofstream ofstream_;

  std::set<std::string> enable_modules_;
private:
	//! 每行日志最大长度
	static const int MAX_LOG_LINE = 102400;

	bool        m_opened;
	//! 日志目录
	char				m_path[1024];

	//! 日志文件名(不包括路径)
	char				m_filename[1024];

	//! 单个日志文件最大行数
	int					m_maxline;

	//! 单个日志文件最大文件大小
	int					m_maxsize;

	//! 日志级别
	unsigned int		m_log_level_flags;

	//! 是否写屏
	bool				m_print_screen_flag;

	//! 是否写文件
	bool				m_print_file_flag;

	//! 当天日志文件序号
	int					m_cur_sn;

	//! 但前日期
	int					m_cur_year;
	int					m_cur_mon;
	int					m_cur_mday;

	//! 当前日志行数
	int					m_cur_line;

	//! 当前日志大小
	int					m_cur_size;

	//! 日志级别描述
	static const char*	m_log_level_desp[];
};

#endif

