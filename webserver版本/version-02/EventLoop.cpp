#include "EventLoop.h"
#include <vector>
#include "Channel.h"
#include <assert.h>
#include <functional>

#include <iostream>
#include "./base/MutexLock.h"
#include <pthread.h>

#include "./base/CurrentThread.h"
__thread EventLoop *t_loopInThisThread = 0;

#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>
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
	  _looping(false),
	  _eventhandling(false),
	  _threadid(CurrentThread::tid()),
	  _wakeupfd(CreateEventFd()),
	  _wakeupchannel(new Channel(this, _wakeupfd)),
	  _quit(false),
	  _callingpendingfunctors(false)
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
	_wakeupchannel->SetEvents(EPOLLIN | EPOLLET);
	_wakeupchannel->SetReadCb(std::bind(&EventLoop::HandleRead, this));
	_wakeupchannel->SetConnCb(std::bind(&EventLoop::HandleConn, this));
	_epoll->Epoll_add(_wakeupchannel, 0);
}

EventLoop::~EventLoop()
{
	close(_wakeupfd);
	t_loopInThisThread = NULL;
}

void EventLoop::Looping()
{
	_looping = true;
	_quit = false;
	std::vector<std::shared_ptr<Channel>> reqs;
	while (!_quit)
	{
		reqs.clear();
		reqs = _epoll->Polling();
		_eventhandling = true;
		for (auto &req : reqs)
		{
			req->handleEvents();
		}
		_eventhandling = false;
		DoPendingFunctor();
		_epoll->HandleExpired();
	}
	_looping = false;
	// pending
}

void EventLoop::AddtoPoll(std::shared_ptr<Channel> chn, int timeout)
{
	// int fd = chn->GetFd();

	_epoll->Epoll_add(chn, timeout);
}

void EventLoop::std::shared_ptr<Channel> chn)
{
	_epoll->Epoll_del(chn);
}

void EventLoop::UpdatetoPoll(std::shared_ptr<Channel> chn, int timeout)
{
	_epoll->Epoll_mod(chn, timeout);
}
void EventLoop::RunInLoop(Functor cb)
{
	if (isInLoopThread())
		cb();
	else
		QueueInLoop(std::move(cb));
}

void EventLoop::QueueInLoop(Functor cb)
{
	std::cout << "new task" << std::endl;
	{
		MutexLockGuard lock(_mutex);
		_functors.push_back(std::move(cb));
	}
	if (!isInLoopThread() || _callingpendingfunctors)
		WakeUp();
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
	_callingpendingfunctors = true;
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
	_callingpendingfunctors = false;
}

void EventLoop::WakeUp()
{
	uint64_t one = 1;
	ssize_t n = writen(_wakeupfd, (char *)(&one), sizeof one);
	if (n != sizeof one)
	{
	}
}

void EventLoop::HandleRead()
{
	uint64_t one = 1;
	ssize_t n = readn(_wakeupfd, &one, sizeof one);

	_wakeupchannel->SetEvents(EPOLLIN | EPOLLET);
}
void EventLoop::HandleConn()
{
	UpdatetoPoll(_wakeupchannel);
}

void EventLoop::Quit()
{
	_quit = true;
	if (!isInLoopThread())
	{
		WakeUp();
	}
}