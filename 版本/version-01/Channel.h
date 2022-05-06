#pragma once
#include <functional>
#include "EventLoop.h"

class EventLoop;

class Channel
{
	using CallBack = typename std::function<void()>;
	CallBack ReadFunction;
	CallBack WriteFunction;
	CallBack ConnFunction;
	CallBack ErrorFunction;

public:
	//构造函数
	Channel(EventLoop *loop, int fd) : _loop(loop), Fd(fd) {}
	Channel(EventLoop* loop):_loop(loop){};


	__uint32_t get_Events(void);

	int get_Fd();

	EventLoop *get_Loop(void);

	void set_Fd(int setfd);

	void set_Event(__uint32_t events);

	void set_Revent(__uint32_t evn);

	void handleEvents();

	void setReadFunction(CallBack&& ReadFun){
		ReadFunction = ReadFun;
	}

	void setConnFunction(CallBack&& ConnFun){
		ConnFunction = ConnFun;
	}

	void setWriteFunction(CallBack&& WriteFun){
		WriteFunction = WriteFun;
	}



private:
	__uint32_t Events;
	__uint32_t Revents;
	int Fd;
	EventLoop *_loop;
};