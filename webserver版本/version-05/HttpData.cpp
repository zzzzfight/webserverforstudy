#include "HttpData.h"
#include "Channel.h"

#include <unistd.h>
#include<functional>

#include <iostream>
#include"EventLoop.h"
HttpData::HttpData(EventLoop *loop, int fd)
	: _channel(new Channel(loop, fd)),
	  _loop(loop),
	  _fd(fd)
{
	// CallBack SetReadCb(CallBack&& funcb);
	// CallBack SetWriteCb(CallBack&& funcb);
	// CallBack SetConnCb(CallBack&& funcb);
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
	char buff[1024];

	int ret = read(_fd, buff, 1024);

	if (ret > 0)
	{
		std::cout << "recv something" << std::endl;
		std::cout << buff << std::endl;
		// close(_fd);
	}
}

void HttpData::DealWithConn()
{
	_channel->SetEvents(DEFAULT_EVENT);
	
}

void HttpData::DealWithWrite()
{
}

void HttpData::NewConn()
{
	// _channel-
	_channel->SetEvents(DEFAULT_EVENT);
	_loop->AddtoPoll(_channel, 0);
}