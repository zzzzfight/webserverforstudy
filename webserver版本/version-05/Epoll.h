#include <sys/epoll.h>
#include <sys/socket.h>
#include "Channel.h"
#include <vector>
#include <memory>

#include "./base/MutexLock.h"

class Epoll
{
public:
	Epoll();

	std::vector<std::shared_ptr<Channel>> Polling();

	void Epoll_add(std::shared_ptr<Channel> chn, int timeout);
	void Epoll_mod(std::shared_ptr<Channel> chn, int timeout);

private:
	int _epollfd;

	// shared_ptr<>
	static const int MAXFDS = 1024;
	std::shared_ptr<Channel> _fd2chan[MAXFDS];
	// std::shared_ptr<Channel>

	// int _listenfd;
	// std::shared_ptr<>
	const static int MAXEVENTS = 1024;
	epoll_event _events[MAXEVENTS];
};