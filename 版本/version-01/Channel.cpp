#include "Channel.h"
using namespace std;

__uint32_t Channel::get_Events()
{
	return Events;
}
void Channel::set_Event(__uint32_t events)
{
	Events = events;
}

void Channel::set_Revent(__uint32_t evn)
{
	Revents = evn;
}

int Channel::get_Fd()
{
	return Fd;
}

void Channel::set_Fd(int setfd)
{
	Fd = setfd;
}

EventLoop *Channel::get_Loop(void)
{
	return _loop;
}

void Channel::handleEvents()
{
	Events = 0;
	if ((Revents & EPOLLHUP) && !(Revents & EPOLLIN))
	{
		Events = 0;
		return;
	}
	if (Revents & EPOLLERR)
	{
		if (ErrorFunction)
			ErrorFunction();
		Events = 0;
		return;
	}
	if (Revents & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
	{
		ReadFunction();
	}
	if (Revents & EPOLLOUT)
	{
		WriteFunction();
	}
	ConnFunction();
}