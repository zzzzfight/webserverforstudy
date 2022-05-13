#include "EventLoopThread.h"
#include <pthread.h>
#include "./base/CountDownLatch.h"
#include <functional>
#include <iostream>
#include "EventLoop.h"
#include <sys/syscall.h>
#include "./base/CurrentThread.h"
#include <unistd.h>
namespace CurrentThread
{
	__thread int t_cachedTid = 0;
	__thread char t_tidString[32];
	__thread int t_tidStringLength = 6;
	__thread const char *t_threadName = "default";
} // namespace CurrentThread

pid_t gettid() { return static_cast<pid_t>(::syscall(SYS_gettid)); }

void CurrentThread::cacheTid()
{
	if (t_cachedTid == 0)
	{
		t_cachedTid = gettid();
		t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
	}
}

EventLoopThread::EventLoopThread()
	: _loop(nullptr),
	  _latch(1) {}

EventLoopThread::~EventLoopThread()
{
	if (_loop)
	{
		_loop->Quit();

		pthread_detach(_threadid);
	}
}

EventLoop *EventLoopThread::ThreadStart()
{
	if (pthread_create(&_threadid, NULL, ThreadFunc, this))
	{
		_started = false;
		return nullptr;
	}
	else
	{
		_latch.wait();
		// std::cout << pthread_self() << ":_2\n";
		_started = true;
		return _loop;
	}
}

void *ThreadFunc(void *obj)
{
	// EventLoopThread *temp = static_cast<EventLoopThread *> obj;
	EventLoopThread *temp = (EventLoopThread *)obj;
	// temp->_pid = gettid();

	// std::cout << pthread_self() << ":_1\n";
	// _latch.countDown();
	EventLoop loop;
	// loop.SetPthreadid(temp->_pid);
	temp->_loop = &loop;
	temp->_latch.countDown();

	// cout << "one loop per thread\n";
	temp->_loop->Looping();

	temp->_loop = nullptr;
	return nullptr;
}