#include "EventLoop.h"
#include <vector>
#include "Channel.h"
#include <assert.h>
#include <functional>

#include <sys/epoll.h>
#include <sys/eventfd.h>

#include <iostream>
#include "./base/MutexLock.h"
#include <pthread.h>

#include "./base/CurrentThread.h"
__thread EventLoop *t_loopInThisThread = 0;

int CreateEventFd()
{
	int evtfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if (evtfd < 0)
	{
		abort();
	}
	return evtfd;
}

EventLoop::EventLoop()
	: _epoll(new Epoll()),
	  _looping(true),
	  _wakeupfd(CreateEventFd()),
	  _pwakeupchannel(new Channel(this, _wakeupfd)),
	  _threadid(CurrentThread::tid())
{
	// _threadid = CurrentThread::tid();
	assert(_threadid);
	// std::cout << "初始化EventLoop" << _threadid << std::endl;
	assert(_epoll > 0);
	if (t_loopInThisThread)
	{
		// LOG << "Another EventLoop " << t_loopInThisThread << " exists in this
		// thread " << threadId_;
	}
	else
	{
		t_loopInThisThread = this;
	}
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
		_epoll->HandleExpired();
	}
	// pending
}

void EventLoop::AddtoPoll(std::shared_ptr<Channel> chn, int timeout)
{
	// int fd = chn->GetFd();

	_epoll->Epoll_add(chn, timeout);
}

void EventLoop::RemovePoll(std::shared_ptr<Channel> chn)
{
	_epoll->Epoll_del(chn)
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
