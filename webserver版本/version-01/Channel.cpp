#include "Channel.h"
#include <sys/epoll.h>
typedef std::function<void()> CallBack;
Channel::Channel(EventLoop *loop, int fd)
	: _mfd(fd),
	  _loop(loop),
	  _events(0),
	  _revents(0),
	  _lastevents(0)
{
}

Channel::Channel(EventLoop *loop)
	: _loop(loop),
	  _events(0),
	  _revents(0),
	  _lastevents(0)
{
}

//监听事件状态设置
unsigned int& Channel::GetEvents()
{
	return _events;
}
void Channel::SetEvents(unsigned int env)
{
	_events = env;
}

unsigned int &Channel::GetREvents(void)
{
	return _revents;
}

void Channel::SetREvents(unsigned int env)
{
	_revents = env;
}

//回调函数设置
void Channel::SetReadCb(CallBack &&funcb)
{
	DealWithRead = funcb;
}
void Channel::SetWriteCb(CallBack &&funcb)
{
	DealWithWrite = funcb;
}
void Channel::SetConnCb(CallBack &&funcb)
{
	DealWithConn = funcb;
}

//文件描述符

void Channel::SetFd(int fd)
{
	_mfd = fd;
}
int Channel::GetFd()
{
	return _mfd;
}

void Channel::handleEvents()
{
	_events = 0;
	if ((_revents & EPOLLHUP) && !(_revents & EPOLLIN))
	{
		_events = 0;
		return;
	}
	if (_revents & EPOLLERR)
	{
		//   if (errorHandler_) errorHandler_();
		_events = 0;
		return;
	}
	if (_revents & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
	{
		DealWithRead();
		
	}
	if (_revents & EPOLLOUT)
	{
		DealWithWrite();
	}
	DealWithConn();
}