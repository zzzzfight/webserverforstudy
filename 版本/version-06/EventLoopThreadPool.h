#pragma once
#include <vector>
#include <memory>
#include "EventLoopThread.h"
// class EventLoopThread

class EventLoop;

class EventLoopThreadPool
{
public:
	EventLoopThreadPool(EventLoop *loop, int threadnum = 8);

	EventLoop *GetNextPool();

	void ThreadPoolInit();

private:
	std::vector<EventLoop *> _loops;
	std::vector<std::shared_ptr<EventLoopThread>> _threads;
	int _threadnum;
	int _next;

	EventLoop *_baseloop;
	bool _started;
	// EventLoop
};