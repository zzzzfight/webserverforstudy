#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include "EventLoop.h"
#include <arpa/inet.h>
#include <string.h>
using namespace std;

class webserver
{
public:
	webserver() {}
	void WebInit()
	{
		_sockfd = socket(AF_INET, SOCK_STREAM, 0);
		// sockaddr_inet socketaddr;
		sockaddr_in socketaddr;
		// bzero((&(char)socketaddr,sizeof(socketaddr)));
		memset(&socketaddr, 0, sizeof(socketaddr));
		socketaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		socketaddr.sin_family = AF_INET;
		socketaddr.sin_port = htons(9006);
		bind(_sockfd, (sockaddr *)&socketaddr, sizeof(socketaddr));
		listen(_sockfd, 5);

		sockaddr_in listenaddr;
		socklen_t addrlen;
		// _listenfd = accept(_sockfd,(sockaddr*)&addrlen,&addrlen);

		// epoll_event _events;
		_epollfd = epoll_create1(1024);
		epoll_ctl(_epollfd, EPOLL_CTL_ADD, _sockfd, _events);
		// accept(_sockfd,)
	}

	void poll()
	{
		int nums = epoll_wait(_epollfd, _events, MAXEVENTNUM, 0);
		for (int i = 0; i < nums; i++)
		{
			int listenfd = _events[i].data.fd;
			if (listenfd == _sockfd)
			{
				if (_events[i].events & EPOLLIN | EPOLLPRI)
				{
					epoll_ctl(_epollfd, EPOLL_CTL_ADD, listenfd, _events);
				}
				else if (_events[i].events & EPOLLERR | EPOLLRDHUP | EPOLLHUP)
				{
					epoll_ctl(_epollfd, EPOLL_CTL_DEL, listenfd, _events);
				}
				// else if(_events[i])
			}
			else
			{
				if (_events[i].events & EPOLLIN)
				{
					dealreadevent(listenfd);
				}else if(_events[i].events&EPOLLOUT){
					dealwritevent(listenfd);
				}
			}
		}
	}

private:
	// EventLoop* Mainloop;
	int _sockfd;
	// int _listenfd;
	int _epollfd;
	const static int MAXEVENTNUM = 1024;
	epoll_event _events[MAXEVENTNUM];

	void dealreadevent(int listenfd);
	void dealwritevent(int listenfd);
};
