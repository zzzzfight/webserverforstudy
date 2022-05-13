#pragma once
#include "Epoll.h"

#include <memory>

#include "Channel.h"
#include <functional>
#include <vector>
#include "./base/CurrentThread.h"
#include <pthread.h>
#include <assert.h>

#include "./base/Util.h"
// test
#include <iostream>
class EventLoop
{
	using Functor = std::function<void()>;

public:
	EventLoop();

	void Looping();

	void AddtoPoll(std::shared_ptr<Channel> chn, int timeout = 0);
	// void AddtoLoop

	void RemovePoll(std::shared_ptr<Channel> chn);

	void UpdatetoPoll(std::shared_ptr<Channel> chn, int timeout = 0);

	void QueueInLoop(Functor cb);
	void DoPendingFunctor();

	bool isInLoopThread() const
	{
		std::cout << this->_threadid << std::endl;

		std::cout << CurrentThread::tid() << std::endl;

		return _threadid == CurrentThread::tid();
	}
	void assertInLoopThread() { assert(isInLoopThread()); }

	const pid_t _threadid;

	void WakeUp()
	{
		uint64_t one = 1;
		ssize_t n = writen()
	}

private:
	std::shared_ptr<Epoll> _epoll;
	// int
	bool _looping;
	int _wakeupfd;
	std::shared_ptr<Channel> _pwakeupchannel;
	std::vector<Functor> _functors;

	MutexLock _mutex;
};