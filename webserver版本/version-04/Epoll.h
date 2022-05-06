#pragma once
#include <sys/epoll.h>
#include <vector>
#include <memory>
#include "Channel.h"
using namespace std;
const int EPOLLWAIT_TIME = 1000;
const int EVENTSUM = 1024;
class Epoll
{
	using SP_Channel = std::shared_ptr<Channel>;
	// typedef std::shared_ptr<Channel> SP_Channel;

public:
	Epoll();
	~Epoll();

	// epoll_create

	std::vector<SP_Channel> Poll();

	void EpollAdd(SP_Channel req, int timeout);
	void EpollMod(SP_Channel req, int timeout);
	void EpollDel(SP_Channel req, int timeout);

	// vector<epoll_event> _events;
	static const int MAXFDS = 1024;
	epoll_event _events[MAXFDS];
	SP_Channel _fd2chan[MAXFDS];

private:
	int _epollfd;
	// epoll_event _event;
	// vector<>
	// vector<SP_Channel> _fd2chan;
	// vector<HttpData> _fd2httpdata;
};