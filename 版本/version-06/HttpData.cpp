#include "HttpData.h"
#include "Channel.h"

#include <unistd.h>
#include <functional>

#include <iostream>
#include "EventLoop.h"

#include "./base/Util.h"
HttpData::HttpData(EventLoop *loop, int fd)
	: _channel(new Channel(loop, fd)),
	  _loop(loop),
	  _fd(fd)
{
	// CallBack SetReadCb(CallBack&& funcb);
	// CallBack SetWriteCb(CallBack&& funcb);
	// CallBack SetConnCb(CallBack&& funcb);
	std::cout << "HTTPDATA:构造" << std::endl;
	_channel->SetReadCb(std::bind(&HttpData::DealWithRead, this));
	_channel->SetWriteCb(std::bind(&HttpData::DealWithWrite, this));
	_channel->SetConnCb(std::bind(&HttpData::DealWithConn, this));
}
// HttpData::HttpData()
// {
// 	_channel->
// }

void HttpData::DealWithRead()
{
	// char buff[1024];

	// std::cout << "hello"; // int ret = read(_fd, _buff, 1024);
	readn(_fd, buff);

	// if (ret > 0)
	// {
	// 	std::cout << "recv something" << std::endl;
	// 	std::cout << _buff << std::endl;
	// 	// close(_fd);
	// }
	// if (write(_fd, _buff, 1024) != 0)
	// {
	// 	unsigned int &events = _channel->GetEvents();
	// 	events = EPOLLIN | EPOLLET;
	// 	_loop->UpdatetoPoll(_channel, 0);
	// 	return;
	// }
	// _channel->SetEvents(EPOLLOUT | EPOLLET);
	unsigned int &events = _channel->GetEvents();
	// std::cout << "_channel:" << _channel << std::endl;
	// std::cout << "events:" << events << std::endl;
	// std::cout << "getrevents:" << _channel->GetREvents() << std::endl;
	events = EPOLLOUT | EPOLLET;
	// std::cout << "EPOLLIN|EPOLLOUT" << (EPOLLOUT | EPOLLET) << std::endl;
	_loop->UpdatetoPoll(_channel, 0);
	// std::cout << "hello";
	return;
}

void HttpData::DealWithConn()
{
	// _channel->SetEvents(DEFAULT_EVENT);
	const char *_buff = buff.c_str();
	int ret = write(_fd, _buff, buff.size());
	assert(ret > 0);
	unsigned int &events = _channel->GetEvents();
	events = EPOLLIN | EPOLLET;
	_loop->UpdatetoPoll(_channel, 0);
	return;
}

void HttpData::DealWithWrite()
{
}

void HttpData::CloseConn()
{
	/
	shared_ptr<HttpData> guard(shared_from_this());
	// _loop
}




void HttpData::NewConn()
{
	// _channel-
	_channel->SetEvents(DEFAULT_EVENT);
	_loop->AddtoPoll(_channel, DEFAULTALIVETIME);
}
