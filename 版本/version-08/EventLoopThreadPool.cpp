#include "EventLoopThreadPool.h"

#include <assert.h>
#include <iostream>
using namespace std;

EventLoopThreadPool::EventLoopThreadPool(EventLoop *loop, int threadnum)
	: _baseloop(loop),
	  _threadnum(threadnum),
	  _next(0),
	  _started(false)
{
	// std::cout << "EventLoopThreadPool_loop:" << loop << endl;
	// std::cout << "EventLoopThreadPool_baseloop:" << _baseloop << std::endl;
	// std::cout << "EventLoopThreadPool_baseloop->_threadid" << _baseloop->_threadid << std::endl;
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
		EventLoop *loop = _curthread->ThreadStart();
		// cout << "_curthread:" << _curthread << endl;
		// cout << "ThreadPoolInit:" << loop << endl;
		_loops.push_back(loop);
		// _loops.push_back(_curthread->ThreadStart());
	}
	if (_threads.size() == _threadnum)
	{
		// for (int i = 0; i < _threadnum; i++)
		// {
		// 	std::cout << _threads[i] << std::endl;
		// }
		std::cout << "threadinitsuccess::" << _threadnum << std::endl;
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
	// std::cout << "GetNextPool:_baseloop" << _baseloop << std::endl;
	// std::cout << "threadnum:" << _threadnum << endl;
	if (!_loops.empty())
	{
		loop = _loops[_next];
		_next = (_next + 1) % _threadnum;
	}
	// std::cout << "curloop:" << loop << endl;

	return loop;
}

// EventLoopThreadPool::