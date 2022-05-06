#pragma once

#include <vector>
#include "EventLoop.h"
#include "EventLoopThread.h"
#include <memory>
class EventLoopThreadPool
{

public:
	EventLoopThreadPool(EventLoop *loop, int _threadnum);
	void ThreadPoolInit();
	EventLoop *GetNextPool();

private:
	std::vector<EventLoop *> _loops;
	std::vector<std::shared_ptr<EventLoopThread>> _threads;
	int _threadnum;
	int _next;

	EventLoop *_baseloop;
	bool _started;
};