#include "EventLoopThread.h"
#include "EventLoop.h"
#include "../../tool/CountDownLatch.h"
#include "CurrentThread.h"
#include <pthread.h>
#include <functional>
// #include <sys/syscall.h>
#include <iostream>
using namespace std;

// pid_t gettid()
// {
// 	return syscall(SYS_gettid);
// }

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
		t_tidStringLength =
			snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
	}
}

EventLoopThread::EventLoopThread()
	: _loop(nullptr),
	  _latch(1) {}
// EventLoopThread::~EventLoopThread() {}

EventLoop *EventLoopThread::ThreadStart()
{
	if (pthread_create(&_pid, NULL, ThreadFunc, this))
	{
		_started = false;
		return nullptr;
	}
	else
	{
		_latch.wait();
		cout << pthread_self() << ":_2\n";
		_started = true;
		return _loop;
	}
}

// EventLoop *EventLoopThread::ThreadStart1()
// {
// 	if (pthread_create(&_pid, NULL, bind(&EventLoopThread::ThreadFunc1, this), NULL))
// 	{
// 		_started = false;
// 		return nullptr;
// 	}
// 	else
// 	{
// 		_latch.wait();
// 		_started = true;
// 		return _loop;
// 	}
// }

// void *EventLoopThread::ThreadFunc1(void *obj)
// {
// 	// EventLoopThread *temp = static_cast<EventLoopThread *> obj;
// 	// EventLoopThread *temp = (EventLoopThread *)obj;
// 	// temp->_pid = gettid();
// 	_latch.countDown();
// 	// _latch.countDown();
// 	EventLoop loop;
// 	// loop.SetPthreadid(temp->_pid);
// 	_loop = &loop;
// 	_loop->Looping();

// 	_loop = nullptr;
// 	return nullptr;
// }

void *ThreadFunc(void *obj)
{
	// EventLoopThread *temp = static_cast<EventLoopThread *> obj;
	EventLoopThread *temp = (EventLoopThread *)obj;
	// temp->_pid = gettid();

	cout << pthread_self() << ":_1\n";
	temp->_latch.countDown();
	// _latch.countDown();
	EventLoop loop;
	// loop.SetPthreadid(temp->_pid);
	temp->_loop = &loop;

	// cout << "one loop per thread\n";
	temp->_loop->Looping();

	temp->_loop = nullptr;
	return nullptr;
}
// EventLoopThread::EventLoopThread(){

// }

// viod *ThreadFunc(void*){
// 	EventLoop loop;
// 	// _loop = &loop;
// 	// loop.loop();

// }
