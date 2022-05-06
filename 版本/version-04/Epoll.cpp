#include "Epoll.h"
#include <iostream>
// #include "Util.h"
#include <sys/epoll.h>
#include "Channel.h"
#include "HttpData.h"
#include <vector>
// #include"Util.h"
#include <assert.h>

#include <iostream>
using namespace std;
using SP_Channel = std::shared_ptr<Channel>;
Epoll::Epoll() : _epollfd(epoll_create(1024))
{
	assert(_epollfd > 0);
}
Epoll::~Epoll() {}

std::vector<SP_Channel> Epoll::Poll()
{
	std::vector<SP_Channel> ret;
	ret.clear();

	epoll_event events[1024];
	int fdindex = epoll_wait(_epollfd, events, EVENTSUM, EPOLLWAIT_TIME);
	// SP_Channel
	// fdindex += 4;
	if (fdindex <= 0)
	{
		// std::cout << "no epoll task" << endl;
		return ret;
		// continue;
	}
	// cout << fdindex << endl;
	for (int i = 0; i < fdindex; i++)
	{
		// cout << "task" << endl;
		int tempfd = events[fdindex].data.fd;
		cout << tempfd << endl;
		SP_Channel cur = _fd2chan[tempfd];

		if (cur)
		{
			cout << "epoll success" << endl;
			cur->SetREvents(events[tempfd].events);
			cur->SetEvents(0);
			ret.push_back(cur);
		}
		else
		{

			// cout << "invaild" << endl;
			//是否需要删除epoll
			cout << "_fd2chan[4]" << _fd2chan[4] << endl;
		}
	}

	return ret;
}

void Epoll::EpollAdd(SP_Channel req, int timeout)
{
	int fd = req->GetFd();
	cout << "epolladd:fd:" << fd << endl;
	//设置非阻塞
	// setSocketNonBlocking(fd);
	// if(timeout>0){
	// 	// AddTimer();
	// 	// _fd2http[fd]
	// }

	epoll_event event;
	event.data.fd = fd;
	cout << "event.data.fd" << event.data.fd << endl;
	event.events = req->GetEvents();

	_fd2chan[fd] = req;
	if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, fd, &event) < 0)
	{
		cout << "epoll_ctl error" << endl;
		_fd2chan[fd].reset();
	}
	else
	{
		cout << "epoll add success" << endl;
	}
}

void Epoll::EpollMod(SP_Channel req, int timeout = 0)
{
	// if(timeout>0)AddTimer()
	//计时器

	int fd = req->GetFd();
	if (!req->EqualAndUpdateLastEvents())
	{
		epoll_event event;
		event.events = req->GetEvents();
		event.data.fd = fd;
		if (epoll_ctl(_epollfd, EPOLL_CTL_MOD, fd, &event) < 0)
		{
			_fd2chan[fd].reset();
		}
	}
	// if()
}
void Epoll::EpollDel(SP_Channel req, int timeout)
{
	int fd = req->GetFd();
	epoll_event event;
	event.data.fd = fd;
	event.events = req->GetEvents();
	if (epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, &event) < 0)
	{
	}
	_fd2chan[fd].reset();
	// _fd2http[fd].reset();
}