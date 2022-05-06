#include <vector>
#include <assert.h>
#include <memory>
#include "EventLoop.h"
#include "EventLoopThread.h"

class EventLoopThreadPool
{
public:
	EventLoopThreadPool(int _threadnum)
		: _threadnum(_threadnum)
	{
	}

	void ThreadPoolInit()
	{
		for (int i = 0; i < _threadnum; i++)
		{
			shared_ptr<EventLoopThread> _curthread(new EventLoopThread());
			_threads.push_back(_curthread);
			// _loop.push_back()
			_loops.push_back(_curthread->startthread());
		}
	}

	EventLoop *getNextPool()
	{
		EventLoop *ret = _loops[_next];
		_next = (ret + 1) / _threadnum;
		return ret;
	}

private:
	// vector<EventLoop>
	std::vector<EventLoop *> _loops;
	std::vector<std::shared_ptr<EventLoopThread>> _threads;
	int _threadnum;
	int _next;
};