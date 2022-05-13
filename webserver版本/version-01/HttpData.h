#pragma once
#include <memory>
#include <sys/epoll.h>
#include <functional>
#include <string.h>
// #include""
class Channel;
class EventLoop;
// class HttpData;
#include <iostream>

using namespace std;







const unsigned int DEFAULT_EVENT = EPOLLIN | EPOLLET | EPOLLONESHOT;
const size_t DEFAULTALIVETIME = 2000;
class HttpData : public std::enable_shared_from_this<HttpData>
{
	// using CallBack = std::function<void()>;

public:
	HttpData(EventLoop *loop, int fd);
	~HttpData()
	{
		cout << "HttpData析构" << endl;
	}
	// HttpData()
	void DealWithRead();
	void DealWithWrite();
	void DealWithConn();

	std::shared_ptr<Channel> GetChannel()
	{
		return _channel;
	}
	void CloseConn();

	void NewConn();

	// std::shared_ptr<Channel> &GetChannel() { return _channel; }

private:
	std::shared_ptr<Channel> _channel;
	// std::weak_ptr<HttpData> _httpdata;
	int _fd;
	EventLoop *_loop;

private:
	// char _buff[1024];
	std::string buff;
};