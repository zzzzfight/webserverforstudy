#pragma once
#include <memory>
#include "Channel.h"
#include <unistd.h>
// #include "EventLoop.h"
// class Channel;
class EventLoop;
class HttpData
{
public:
	HttpData(EventLoop *loop, int connfd);
	~HttpData() { close(_mfd); }

	void HandlerRead();
	void HandlerWrite();
	void HandlerConn();

	std::shared_ptr<Channel> &GetChannel()
	{
		return _channel;
	}
	void NewEvent();

private:
	std::shared_ptr<Channel> _channel;
	EventLoop *_loop;
	int _mfd;
};