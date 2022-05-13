#pragma once

#include "./base/CountDownLatch.h"
// #include<functional>
#include <pthread.h>
// class EventLoop;
#include "EventLoop.h"

void *ThreadFunc(void *obj);

class EventLoopThread
{
public:
	EventLoopThread();
	~EventLoopThread();
	EventLoop *ThreadStart();

	EventLoop *_loop;

	pthread_t _threadid;

	bool _started;
	CountDownLatch _latch;

private:
};