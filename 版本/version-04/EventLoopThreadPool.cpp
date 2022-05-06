#include "EventLoopThreadPool.h"
#include "EventLoop.h"

#include <memory>

#include <assert.h>
#include <iostream>
using namespace std;
// pthread_once_t

EventLoopThreadPool::EventLoopThreadPool(EventLoop *loop, int threadnum)
	: _baseloop(loop),
	  _threadnum(threadnum),
	  _next(0),
	  _started(false)
{
	// if(thread)
}

void EventLoopThreadPool::ThreadPoolInit()
{

	// for (int i = 0; i < _threadnum; i++)
	// {
	// 	std::shared_ptr<EventLoopThread> _curthread(new EventLoopThread());
	// 	// _threads.push_back(_curthread);

	// 	// _loops.push_back(_curthread->StartThread());
	// }
	_baseloop->assertInLoopThread();
	_started = true;
	for (int i = 0; i < _threadnum; i++)
	{
		std::shared_ptr<EventLoopThread> _curthread(new EventLoopThread());
		_threads.push_back(_curthread);
		_loops.push_back(_curthread->ThreadStart());
	}
	if (_threads.size() == _threadnum)
	{
		for (int i = 0; i < _threadnum; i++)
		{
			cout << _threads[i] << endl;
		}
		cout << "threadinitsuccess::" << _threadnum << endl;
	}
}

EventLoop *EventLoopThreadPool::GetNextPool()
{
	_baseloop->assertInLoopThread();
	assert(_started);
	// EventLoop *loop = _loops[_next];
	// cout << _next << endl;
	// _next = (_next + 1) % _threadnum;
	EventLoop *loop = _baseloop;
	if (!_loops.empty())
	{
		loop = _loops[_next];
		_next = (_next + 1) % _threadnum;
	}

	return loop;
}

// EventLoopThreadPool::