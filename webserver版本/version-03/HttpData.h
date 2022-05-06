#pragma once
#include "EventLoop.h"
#include "Channel.h"
class Channel;
class EventLoop;
class HttpData
{
public:
	HttpData() {}
	std::shared_ptr<Channel> &GetChannel()
	{
		return _channel;
	}

private:
	void dealwithread();
	void dealwithwrite();
	EventLoop *_loop;
	std::shared_ptr<Channel> _channel;

	// void dealwithconn();
};