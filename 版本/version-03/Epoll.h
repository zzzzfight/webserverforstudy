#pragma once

// #include<sys/epoll.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <vector>
#include <memory>
// #define MAXEVENTNUM 1024
#include "Channel.h"
class Epoll
{
public:
	Epoll() : epollfd(epoll_create(5))
	{
	}

	~Epoll()
	{
	}
	std::vector<std::shared_ptr<Channel>> poll()
	{
		int fdnums = epoll_wait(epollfd, Events, MAXFDS, 0);
		std::vector<std::shared_ptr<Channel>> ret;
		for (int i = 0; i < fdnums; i++)
		{
			int fdIndex = Events[i].data.fd;
			std::shared_ptr<Channel> curRequest = Fd2Chann[fdIndex];
			if (curRequest)
			{
				// curRequest->setEvents()
				/*
				
				*/ 
				ret.push_back(curRequest);
			}
		}

		return ret;
	}

private:
	const int static MAXFDS = 1024;
	int epollfd;

	// bool quit_;
	// epoll_event Events[MAXFDS];
	// std::vector<epoll_event> Events;
	epoll_event Events[MAXFDS];
	std::shared_ptr<Channel> Fd2Chann[MAXFDS];
	// std::shared_ptr<HTTP> Fd2Http[MAXFDS];
};