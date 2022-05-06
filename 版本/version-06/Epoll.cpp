#include "Epoll.h"
#include <vector>
#include <assert.h>
#include <iostream>
#include "Channel.h"
// test
#include <errno.h>
#include <iostream>
#include <string.h>
#include "./Timer.h"
Epoll::Epoll()
	: _epollfd(epoll_create(1024))
{
}

void Epoll::Epoll_add(std::shared_ptr<Channel> chn, int timeout)
{
	int curfd = chn->GetFd();
	if (timeout > 0)
	{
		std::shared_ptr<HttpData> http = chn->GetHolder();
		if (http)
		{
			_fd2http[curfd] = http;
			_timemanager.add_timer(chn, timeout);
		}
		else
		{
			abort();
		}
		// _fd2http[curfd] = chn->GetHolder();
	}
	epoll_event events;
	events.data.fd = curfd;
	events.events = chn->GetEvents();

	assert(_epollfd);
	assert(curfd);
	// std::cout << "Epoll_curfd::" << curfd << std::endl;
	int ret = epoll_ctl(_epollfd, EPOLL_CTL_ADD, curfd, &events);

	// assert(ret == 0);

	char *mesg = strerror(errno);
	perror(mesg);

	_fd2chan[curfd] = chn;
}

void Epoll::Epoll_mod(std::shared_ptr<Channel> chn, int timeout)
{

	int curfd = chn->GetFd();
	epoll_event events;
	events.data.fd = curfd;
	events.events = chn->GetEvents();
	int ret = epoll_ctl(_epollfd, EPOLL_CTL_MOD, curfd, &events);
	assert(ret == 0);
	_fd2chan[curfd] = chn;
}

void Epoll::Epoll_del(std::shared_ptr<Channel> chn)
{
	int curfd = chn->GetFd();
	epoll_event events;
	events.data.fd = curfd;
	events.events = chn->GetEvents();
	int ret = epoll_ctl(_epollfd, EPOLL_CTL_DEL, curfd, &events);
	assert(ret = 0);
	_fd2chan[curfd].reset();
	_fd2http[curfd].reset();
}

std::vector<std::shared_ptr<Channel>> Epoll::Polling()
{
	std::vector<std::shared_ptr<Channel>> ret;
	int eventnum = epoll_wait(_epollfd, _events, MAXEVENTS, 1000);

	for (int i = 0; i < eventnum; ++i)
	{
		int curfd = _events[i].data.fd;
		assert(curfd > 0);
		if (_fd2chan[curfd])
		{
			_fd2chan[curfd]->SetREvents(_events[i].events);
			_fd2chan[curfd]->SetEvents(0);
			ret.push_back(_fd2chan[curfd]);
		}
		else
		{
			std::cout << " epoll error " << std::endl;
		}
	}

	return ret;
}

void Epoll::HandleExpired()
{
	_timemanager.HandleExpiredEvent();
}