#include "Channel.h"
#include <sys/epoll.h>

Channel::Channel(EventLoop *loop)
	: _baseloop(loop)
{
}
Channel::Channel(EventLoop *loop, int fd)
	: _baseloop(loop),
	  _mfd(fd)
{
}

void Channel::SetFd(int fd)
{
	_mfd = fd;
}
int Channel::GetFd()
{
	return _mfd;
}

void Channel::SetEvents(unsigned int evnt)
{
	Events = evnt;
}
void Channel::SetREvents(unsigned int evnt)
{
	REvents = evnt;
}

unsigned int Channel::GetEvents()
{
	return Events;
}
unsigned int Channel::GetREvents()
{
	return REvents;
}

void Channel::SetReadHandler(callback &&cb)
{
	ReadHandler = cb;
}
void Channel::SetWriteHandler(callback &&cb)
{
	WriteHandler = cb;
}
void Channel::SetConnHandler(callback &&cb)
{
	ConnHanndler = cb;
}

void Channel::EventHandler()
{
	Events = 0;
	if ((REvents & EPOLLHUP) && !(REvents & EPOLLIN))
	{
		Events = 0;
		return;
	}
	if (REvents & EPOLLERR)
	{
		if (ErrorHandler)
			ErrorHandler();
		Events = 0;
		return;
	}
	if (REvents & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
	{
		ReadHandler();
	}
	if (REvents & EPOLLOUT)
	{
		WriteHandler();
	}
	ConnHanndler();
}

void Channel::SetHolder(std::shared_ptr<HttpData> http)
{
	_http = http;
}