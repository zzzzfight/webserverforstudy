#include "Epoll.h"
#include <vector>
#include <assert.h>
#include <iostream>
#include "Channel.h"

Epoll::Epoll()
	: _epollfd(epoll_create(1024))
{
}

void Epoll::Epoll_add(std::shared_ptr<Channel> chn, int timeout)
{
	int curfd = chn->GetFd();
	epoll_event events;
	events.data.fd = curfd;
	events.events = chn->GetEvents();
	int ret = epoll_ctl(_epollfd, EPOLL_CTL_ADD, curfd, &events);
	assert(ret == 0);
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