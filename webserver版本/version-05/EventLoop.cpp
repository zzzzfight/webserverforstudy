#include "EventLoop.h"
#include <vector>
#include "Channel.h"
#include <assert.h>
#include <functional>

#include <iostream>
#include "./base/MutexLock.h"

EventLoop::EventLoop()
	: _epoll(new Epoll()),
	  _looping(true)
{
	assert(_epoll > 0);
}

void EventLoop::Looping()
{
	while (_looping)
	{
		std::vector<std::shared_ptr<Channel>> reqs;
		reqs = _epoll->Polling();

		for (auto &req : reqs)
		{
			req->handleEvents();
		}
		DoPendingFunctor();
	}
	// pending
}

void EventLoop::AddtoPoll(std::shared_ptr<Channel> chn, int timeout)
{
	// int fd = chn->GetFd();
	_epoll->Epoll_add(chn, 0);
}

void EventLoop::UpdatetoPoll(std::shared_ptr<Channel> chn, int timeout)
{
	_epoll->Epoll_mod(chn, 0);
}

void EventLoop::QueueInLoop(Functor cb)
{
	std::cout << "new task" << std::endl;
	MutexLockGuard lock(_mutex);
	_functors.push_back(std::move(cb));
}

// void EventLoop::QueueInLoop(callback &&cb)
// {
// 	MutexLockGuard mutex(_lock);
// 	_functors.push_back(std::move(cb));
// 	cout << "new task" << endl;
// }

void EventLoop::DoPendingFunctor()
{
	std::vector<Functor> functors;
	{
		MutexLockGuard lock(_mutex);
		functors.swap(_functors);
	}
	// for (auto curfun : _functors)
	// {
	// 	curfun();
	// }
	for (int i = 0; i < functors.size(); i++)
	{
		functors[i]();
		std::cout << "fuctor" << std::endl;
	}
}
