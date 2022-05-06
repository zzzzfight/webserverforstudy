#include"EventLoop.h"

void EventLoop::loop(void)
{

	while (true)
	{
		// int fdnum = epoll_wait(Sockfd, Events, MaxFd, -1);
		
		std::vector<SP_Channel> ret = _epoll->poll();
		for(auto &p:ret){
			// p->WriteFunction()
			p->handleEvents();
		}

	}
}