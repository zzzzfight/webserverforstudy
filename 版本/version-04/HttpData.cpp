#include "HttpData.h"
#include "EventLoop.h"
#include "Channel.h"
#include <iostream>
#include <sys/epoll.h>
#include <errno.h>
const __uint32_t DEFAULT_EVENT = EPOLLIN | EPOLLET | EPOLLONESHOT;
const int DEFAULT_EXPIRED_TIME = 2000;
HttpData::HttpData(EventLoop *loop, int connfd)
	: _mfd(connfd),
	  _channel(new Channel(loop, connfd))
{
	_channel->SetReadHandler(bind(&HttpData::HandlerRead, this));
	_channel->SetWriteHandler(bind(&HttpData::HandlerWrite, this));
	_channel->SetConnHandler(bind(&HttpData::HandlerConn, this));
}

void HttpData::HandlerRead()
{
	std::cout << "this is a read test" << std::endl;
	char buffer[1024];
	while (true)
	{
		read(_mfd, buffer, 1024);
		if (errno == EINTR)
		{
			continue;
		}
		else
		{
			break;
		}
	}
	_channel->SetEvents(EPOLLOUT | EPOLLET);
	_loop->UpdataEpoll(_channel, 0);
	return;
}
void HttpData::HandlerWrite()
{
	// write(_mfd,)
	char *buffer = "helloworld";
	write(_mfd, buffer, 1024);
	// close(_mfd);
}
void HttpData::HandlerConn()
{
	std::cout << "this is a conn test" << std::endl;
}

void HttpData::NewEvent()
{
	_channel->SetEvents(DEFAULT_EVENT);
	// _loop->addToPoller(channel_, DEFAULT_EXPIRED_TIME);
	_loop->AddToEpoll(_channel, DEFAULT_EXPIRED_TIME);
}