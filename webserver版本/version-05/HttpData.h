#pragma once
#include <memory>
#include <sys/epoll.h>
#include <functional>
// #include""
class Channel;
class EventLoop;

const __uint32_t DEFAULT_EVENT = EPOLLIN | EPOLLET | EPOLLONESHOT;
class HttpData
{
	// using CallBack = std::function<void()>;

public:
	HttpData(EventLoop *loop, int fd);
	// HttpData()
	void DealWithRead();
	void DealWithWrite();
	void DealWithConn();

	std::shared_ptr<Channel> GetChannel()
	{
		return _channel;
	}

	void NewConn();

private:
	std::shared_ptr<Channel> _channel;
	int _fd;
	EventLoop *_loop;
};