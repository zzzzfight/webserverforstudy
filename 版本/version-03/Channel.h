#pragma once
#include <functional>
#include <memory>
#include"HttpData.h"

// #include "Http.h"
#include "EventLoop.h"
class HttpData;
class EventLoop;
class Channel
{
public:
	Channel(EventLoop *loop, int fd)
	{
		_mfd = fd;
		_loop = loop;
	}

	Channel(EventLoop *loop)
	{
		this->_loop = _loop;
	}

	using func_cb = typename std::function<void(void)>;
	// func_cb DealWithConn;

	void setEvents(unsigned int evnt)
	{
		Events = evnt;
	}
	unsigned int getEvents()
	{
		return Events;
	}

	void setREvents(unsigned int evnt)
	{
		REvents = evnt;
	}
	unsigned int getREvents()
	{
		return REvents;
	}

	void setReadCB(func_cb &&cb)
	{
		DealWithRead = cb;
	}
	void setWriteCB(func_cb &&cb)
	{
		DealWithWrite = cb;
	}
	void setConnCB(func_cb &&cb)
	{
		DealWithConn = cb;
	}
	void setFd(int fd){
		_mfd = fd;
	}
	void SetHolder(std::shared_ptr<HttpData>& http){
		_http = http;
	}
private:
	int _mfd;
	int state;
	unsigned int REvents; //存储当前事件REvents
	unsigned int Events;  //存储需要修改的事件Events
	func_cb DealWithRead;
	func_cb DealWithWrite;
	func_cb DealWithConn;
	HttpData *_http;
	EventLoop *_loop;

	// weak
	std::weak_ptr<HttpData> _http;
};
