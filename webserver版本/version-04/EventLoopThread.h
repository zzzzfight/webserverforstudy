#pragma once
#include "EventLoop.h"
#include "../../tool/CountDownLatch.h"

#include <pthread.h>
#include <functional>
#include <sys/syscall.h>
#include <unistd.h>
void *ThreadFunc(void *obj);
class EventLoopThread
{
public:
	EventLoopThread();

	EventLoop *ThreadStart();

	EventLoop *_loop;
	pthread_t _pid;
	bool _started;
	CountDownLatch _latch;

	// void *EventLoopThread::ThreadFunc1(void *obj);
	// EventLoop *ThreadStart1();
private:
};

// void *ThreadFunc(void *obj)
// {
// 	// EventLoopThread *temp = static_cast<EventLoopThread *> obj;
// 	EventLoopThread *temp = (EventLoopThread *)obj;
// 	temp->_latch.countDown();
// 	// _latch.countDown();
// 	EventLoop loop;
// 	temp->_loop = &loop;
// 	temp->_loop->Looping();

// 	temp->_loop = nullptr;
// 	return nullptr;
// }