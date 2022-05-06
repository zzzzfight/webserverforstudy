#pragma once

#include <functional>

//状态/

typedef std::function<void()> CallBack;
class EventLoop;
class Channel
{
	// using CallBack = typename std::function<void()>;

public:
	Channel(EventLoop *loop);
	Channel(EventLoop *loop, int fd);

	//回调函数
	CallBack DealWithRead;
	CallBack DealWithWrite;
	CallBack DealWithConn;

	void SetReadCb(CallBack &&funcb);
	void SetWriteCb(CallBack &&funcb);
	void SetConnCb(CallBack &&funcb);

	void SetEvents(unsigned int env);
	unsigned int GetEvents(void);

	void SetREvents(unsigned int env);
	unsigned int GetREvents(void);

	int GetFd();
	void SetFd(int fd);

	void handleEvents();

private:
	EventLoop *_loop;
	int _mfd;

	unsigned int _events;
	unsigned int _revents;
	unsigned int _lastevents;
};