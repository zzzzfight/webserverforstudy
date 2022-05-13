// @Author Z
// @Email 3161349290@qq.com
#include <sys/epoll.h>
#include <sys/socket.h>
// #include"Channel.h"

#include <vector>
#include <memory>

class Channel;

#include "./base/MutexLock.h"

// static const int MAXFDS = 1024;
// static const int MAXEVENTS = 1024;

class Epoll
{
	Epoll();
	~Epoll();

	void EpollAdd();
	void EpollMod();
	void EpollDel();

private:
	int _epollfd;
};