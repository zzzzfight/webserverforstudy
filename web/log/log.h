#ifndef LOG_H
#define LOG_H
#include<iostream>
#include<cstdio>
#include"block_queue.h"
// #include<cstdarg>
// #include<cstring>
// #include<sys/time.h>
#include"../locker/locker1.h"
using namespace std;
// enum LEVEL_STATUS{INFO = 0, WARN};
class Log{
public:
	bool log_Init(const char*, int, int, int);

	static Log* get_instance();

	void log_Write(int level, const char* context, ...);

	static void log_Des();

	static void* flush_log_thread(void* args){
		Log::get_instance()->async_write_log();
		return nullptr;
	}

	int que_size(){
		return que->que_size();
	}

	// bool que_clear(){
	// 	string temp;
	// 	while(que->queue_clear(temp)){
	// 		m_mutex.lock();
	// 		fputs(temp.c_str(), m_fp);
	// 		std::fflush(m_fp);
	// 		m_mutex.unlock();
	// 	}
	// 	return true;
	// }
private:
	void *async_write_log();

	Log(){
		m_count = 0;
	}
	~Log(){
		delete[] m_buffer;
		fclose(m_fp);
		if(que){
			delete que;
		}
	}
	//virtual ~Log();

public:
	
	void flush(void);


private:
	char* m_buffer;
	static Log *instance;
	locker m_mutex;
	int m_today;
	FILE* m_fp;
	char dir_name[128];
	char log_name[128];
	int m_log_buf_size;
	int m_split_lines;
	long long m_count = 0;
	bool if_async = false;
	blockQueue<string>* que;
};

#define LOG_INIT(path, buffer_size, log_count, que_size){Log::get_instance()->log_Init(path, buffer_size, log_count, que_size);}
#define LOG_DEST() Log::log_Des()
#define LOG_INFO(format, ...) {Log::get_instance()->log_Write(0, format, ##__VA_ARGS__); Log::get_instance()->flush();}
#define LOG_WARN(format, ...) {Log::get_instance()->log_Write(1, format, ##__VA_ARGS__); Log::get_instance()->flush();}
#endif