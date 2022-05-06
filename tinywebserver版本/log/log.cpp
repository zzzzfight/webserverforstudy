#include<iostream>
#include<cstdarg>
#include<cstring>
#include<time.h>
#include<pthread.h>
#include"../locker/locker.h"
#include"log.h"
#include"block_queue.h"
#include<thread>


#include<unistd.h>
using namespace std;

Log* Log::instance = nullptr;

Log* Log::get_instance(){
	if(instance == nullptr){
		instance = new Log();
	}
	return instance;
}


// void Log::log_Init
bool Log::log_Init(const char* file_name, int log_buf_size, int split_lines,  int que_size){
	m_log_buf_size = log_buf_size;
	m_buffer = new char[m_log_buf_size];
	memset(m_buffer, '\0', m_log_buf_size);
	m_split_lines = split_lines;

	if (que_size >= 1)
    {
        if_async = true;
        que = new blockQueue<string>(que_size);
        pthread_t tid;
        // flush_log_thread为回调函数,这里表示创建线程异步写日志
        pthread_create(&tid, NULL, Log::flush_log_thread, NULL);
		// pthread_detach(tid);
		// thread(flush_log_thread);
    }



	time_t t = time(NULL);
	struct tm* sys_tm = localtime(&t);
	struct tm my_tm = *sys_tm;

	const char* p = strrchr(file_name, '/');
	char log_full_name[512] = {0};
	if(p == NULL){
		snprintf(log_full_name, 511, "%d_%02d_%02d_%s", my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday, file_name);
	}else{
		strcpy(log_name, p + 1);
		
		strncpy(dir_name, file_name, p - file_name + 1);

		snprintf(log_full_name, 511, "%s%d_%02d_%02d_%s", dir_name, my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday, log_name);
		
	}
	m_today = my_tm.tm_mday;

	m_fp = fopen(log_full_name, "a");
	if(m_fp){
		return true;
	}
	return false;

}

void Log::log_Des(){
	LOG_INFO("destory the log_system");
	
	instance->flush();
	// flush_log_thread(nullptr);
	delete instance;
}
void Log::log_Write(int level,const char* format,...){
	// char buffer[1024] = {0};
	
	/*
	//linux内置的time_api<sys/time.h>
	struct timeval now = {0 ,0};
	gettimeofday(&now, NULL);
	time_t t = now.tv_sec;
	struct tm *sys_tm = localtime(&t);
	struct tm my_tm = *sys_tm;
	//
	*/
	
	//标准C库
	time_t t =time(NULL);
	// time(&t);
	struct tm* sys_tm = localtime(&t);
	struct tm my_tm = *sys_tm;
	char s[8] = {0};

	switch(level){
		case 0:
			strcpy(s,"[info]:");
			break;
		case 1:
			strcpy(s,"[warn]:");
			break;
		case 2:
			strcpy(s, "[erro]");
			break;
		case 3:
			strcpy(s, "[debug]");
			break;
		default:
			strcpy(s, "[info]");
			break;
	}
	m_mutex.lock();
	m_count++;
	if(m_today!=my_tm.tm_mday || m_count%m_split_lines == 0){
		char new_log[512] = {0};
		fflush(m_fp);
		fclose(m_fp);
		char tail[16] = {0};

		snprintf(tail, 16, "%d_%02d_%02d_", my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday);
		if(m_today != my_tm.tm_mday){
			snprintf(new_log, 511, "%s%s%s", dir_name, tail, log_name);	
			m_today = my_tm.tm_mday;
			m_count = 0;
		}else{
			snprintf(new_log, 511, "%s%s%s.%lld", dir_name, tail, log_name, m_count / m_split_lines);
		}
		m_fp = fopen(new_log, "a");
	}


	m_mutex.unlock();
	va_list v1;
	va_start(v1, format);
	string log_str;
	m_mutex.lock();

	int n = snprintf(m_buffer, 80, "%d-%02d-%02d %02d:%02d:%02d.%06d %s ",
                     my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday,
                     my_tm.tm_hour, my_tm.tm_min, my_tm.tm_sec, my_tm.tm_sec, s);

	int m = vsnprintf(m_buffer + n, 20, format, v1);
	
	// m_mutex.lock();

	m_buffer[m + n] = '\n';
	m_buffer[n + m + 1] = '\0';
	log_str = m_buffer;
	m_mutex.unlock();

	va_end(v1);
	m_mutex.lock();
	if(if_async && !que->queue_isfull()){
		que->queue_push(log_str);
	}else{
		fputs(log_str.c_str(), m_fp);
	}
	m_mutex.unlock();
}
void* Log::async_write_log(){
	string single_log;
	while(que->queue_pop(single_log))
	{
		m_mutex.lock();
		fputs(single_log.c_str(), m_fp);
		// fflush(m_fp);
		m_mutex.unlock();
	}
	return nullptr;
}

void Log::flush(void){
	m_mutex.lock();
	fflush(m_fp);
	m_mutex.unlock();
}

int main(){
// 	// Log* newlog = Log::get_instance("./log_file/log.txt", 1024, 100);
// 	// newlog->log_Write(0, "helloworld");
// 	// Log::get_instance()->log_Init("./log_file/log.txt", 1024, 10);
	LOG_INIT("./log_file/log.txt", 1024, 10, 5);
	
	LOG_INFO("helloworld");
	LOG_INFO("helloworld");
	LOG_INFO("helloworld");
	LOG_INFO("helloworld");
	LOG_INFO("helloworld");
	LOG_INFO("helloworld");
	LOG_INFO("helloworld");
	LOG_INFO("helloworld");
	LOG_INFO("helloworld");
	LOG_INFO("helloworld");
	LOG_INFO("helloworld");
	LOG_INFO("helloworld");
	cout<<Log::get_instance()->que_size()<<endl;
	LOG_DEST();
}


