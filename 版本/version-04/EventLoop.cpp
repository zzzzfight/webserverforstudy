// test
#include "EventLoop.h"
#include "Channel.h"
#include <memory>

// #include<sys/eventfd.h>
// #include <sys/epoll.h>
// #include <iostream>
// #include "Util.h"
// test
#include <vector>
#include <pthread.h>
#include <iostream>
using namespace std;
EventLoop::EventLoop()
	: _epoll(new Epoll()),
	  _threadid(CurrentThread::tid())
{
}
EventLoop::~EventLoop()
{
	// delete _epoll;
}

void EventLoop::Looping()
{
	// std::cout << "looping:" << pthread_self() << std::endl;
	// _pid = pthread_self();
	_looping = true;
	quit = false;
	std::vector<std::shared_ptr<Channel>> ret;
	while (!quit)
	{
		// cout << pthread_self() << endl
		//  << flush;
		ret.clear();
		// if(_fd2chan.size())cout<<_fd2chan.size()<<endl;
		ret = _epoll->Poll();

		// if (!ret.size())
		// {~
		// 	// cout << "has no task" << endl;
		// }

		for (auto cur : ret)
		{
			cout<<"EventLoop::deal a req"<<endl;
			cur->EventHandler();
		}
		// DoPendingFunctor();
		// if(t)
		DoPendingFunctor();
	}
}

void EventLoop::UpdataEpoll(shared_ptr<Channel> channel, int timeout = 0)
{
	// _loop
	_epoll->EpollMod(channel, timeout);
}
void EventLoop::AddToEpoll(shared_ptr<Channel> channel, int timeout = 0)
{
	_epoll->EpollAdd(channel, timeout);
	cout << "EpollAdd" << endl;
}

// void EventLoop::RunInLoop(callback cb)
// {
// 	// MutexLock
// 	MutexLockGuard mutex(_lock);
// 	// _lock.lock();
// 	_functors.push_back(cb);
// }

void EventLoop::QueueInLoop(callback &&cb)
{
	MutexLockGuard mutex(_lock);
	_functors.push_back(std::move(cb));
	cout << "new task" << endl;
}

void EventLoop::DoPendingFunctor()
{
	vector<callback> functors;
	{
		MutexLockGuard mutex(_lock);
		functors.swap(_functors);
	}
	// for (auto curfun : _functors)
	// {
	// 	curfun();
	// }
	for (int i = 0; i < functors.size(); i++)
	{
		functors[i]();
		cout << "fuctor" << endl;
	}
}
