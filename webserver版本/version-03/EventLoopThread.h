// #include"Thread.h"
#include "EventLoop.h"
#include "pthread.h"
#include "../../tool/CountDownLatch.h"
#include "CurrentThread.h"
#include <unistd.h>
#include <iostream>
using namespace std;

namespace CurrentThread
{
	__thread int t_cachedTid = 0;
	__thread char t_tidString[32];
	__thread int t_tidStringLength = 6;
	__thread const char *t_threadName = "default";
}

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

class EventLoopThread
{
public:
	EventLoopThread()
		: _loop(nullptr),
		  _threadname("EventLoop Thread"),
		  _latch(1)
	{
	}
	void *ThreadFunc(void *)
	{
		_tid = CurrentThread::cacheTid();
		cout << "thread:" << _tid << "success" << endl;
		// _latch.countdown();
		_latch.countdown();

		EventLoop loop;

		_loop = &loop;

		loop.loop();

		CurrentThread::t_threadName = "finished";
	}

	void join();
	EventLoop *startthread()
	{

		if (pthread_create(&_pid, NULL, ThreadFunc, NULL))
		{
			_started = false;
		}
		else
		{
			_latch.wait();
			_started = true;
		}
		return _loop;
	}

private:
	// pool
	EventLoop *_loop;
	// Thread _thread;
	bool _started;
	// bool

	pthread_t _pid;
	pid_t _tid;
	std::string _threadname;
	CountDownLatch _latch;
};