#pragma once
#ifndef LOG_HPP_GBWT6OGK
#define LOG_HPP_GBWT6OGK

#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <ctime>
#include <iomanip>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <sovlyn/utility/Singleton.hpp>
#include <sstream>
#include <mutex>

namespace sovlyn{
namespace utility{
class Logger final{
	public:
		enum Level {
			DEBUG = 0,
			INFO,
			WARN,
			ERROR,
			FATAL,
			LEVEL_COUNT
		};
		void log(Level level, const char * file, int line, const char * format, ...);
		Logger(){}
		~Logger(){m_fout.close();}
		void open(const std::string & filename);
		void level(Level level){m_level=level;}
		void max(int m){m_max=m;}

	private:
		Level m_level=DEBUG;
		int m_len=0;
		int m_max=0;
		int m_no=0;
		std::string m_filename;
		std::ofstream m_fout;
		static inline const char * s_level[LEVEL_COUNT]={"DEBUG", "INFO", "WARN", "ERROR", "FATAL"};
		std::mutex m_lock;

		void rotate();
};

void Logger::log(Logger::Level level, const char * file, int line, const char * format, ...){
	m_lock.lock();
	if(m_level > level)return;
	if(!m_fout.is_open()){
		throw std::logic_error("open log file faild: "+(m_filename.empty()?"no log file":m_filename));
	}
	std::stringstream ss;
	std::time_t t = std::time(nullptr);
	std::tm * tm = std::localtime(&t);
	ss << std::put_time(tm, "%Y-%m-%d %H:%M:%S")<<" "<<s_level[level]<<" "<<file<<":"<<line<<" ";

	std::va_list arg_list;
	va_start(arg_list, format);
	int size = vsnprintf(NULL, 0, format, arg_list);
	va_end(arg_list);
	if(size>0){
		char * content = new char[size+1];
		memset((void *)content, 0, sizeof(content));
		va_start(arg_list, format);
		vsnprintf(content, size+1, format, arg_list);
		va_end(arg_list);
		ss<<content;
		delete[] content;
	}else{
		ss<<"fail to parse format";
	}
	std::string s=ss.str();
	m_len+=s.size();
	m_fout<<s<<std::endl;
	if(m_max>0&&m_len>=m_max){
		rotate();
	}
	m_lock.unlock();
}

void Logger::open(const std::string & filename){
	m_filename=filename;
	m_fout.open(filename, std::ios::app);
	if(m_fout.fail()){
		throw std::logic_error("open log file faild: "+filename);
	}
	m_fout.seekp(0, std::ios::end);
	m_len=m_fout.tellp();
}

void Logger::rotate(){
	m_fout.close();
	std::stringstream ss;
	std::time_t t = std::time(nullptr);
	std::tm * tm = std::localtime(&t);
	ss <<m_filename<<'.'<<m_no<< std::put_time(tm, ".%Y-%m-%d %H:%M:%S");
	if(rename(m_filename.c_str(), ss.str().c_str())!=0){
		throw std::logic_error("rename log file failed: "+std::string(strerror(errno)));
	}
	open(m_filename);
	m_len=0;
	m_no++;
}

};
};

#define L_DEBUG(format, ...) \
	sovlyn::utility::Singleton<Logger>::instance().log(sovlyn::utility::Logger::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define L_INFO(format, ...) \
	sovlyn::utility::Singleton<Logger>::instance().log(sovlyn::utility::Logger::INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define L_WARN(format, ...) \
	sovlyn::utility::Singleton<Logger>::instance().log(sovlyn::utility::Logger::WARN, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define L_ERROR(format, ...) \
	sovlyn::utility::Singleton<Logger>::instance().log(sovlyn::utility::Logger::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define L_FATAL(format, ...) \
	sovlyn::utility::Singleton<Logger>::instance().log(sovlyn::utility::Logger::FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__)
#endif /* end of include guard: LOG_HPP_GBWT6OGK */
