#pragma once
#include <iostream>
#include <pthread.h>
#include <chrono>
#include <vector>
// test
#include <memory>
#include <functional>
#include "Channel.h"
#include "Epoll.h"
// #include"HttpData.h"
#include "../../tool/MutexLock.h"
#include "CurrentThread.h"

#include <assert.h>
using namespace std;
class EventLoop
{
	using callback = function<void(void)>;

public:
	EventLoop();
	~EventLoop();
	void Looping();

	bool _looping = false;
	bool quit = false;

	// void UpdataLoop();
	void UpdataEpoll(std::shared_ptr<Channel> channel, int timeout);
	void AddToEpoll(std::shared_ptr<Channel> channel, int timeout);
	// void SetPthreadid(pthread_t pid)
	// {
	// 	_pid = pid;
	// }

	// void RunInLoop();
	void DoPendingFunctor();
	void QueueInLoop(callback &&cb);

	pthread_t _pid;
	// vector<Channel> _fd2chan;
	shared_ptr<Epoll> _epoll;
	MutexLock _lock;
	// vector<HttpData>
	const pid_t _threadid;
	bool isInLoopThread() const { return _threadid == CurrentThread::tid(); }
	void assertInLoopThread() { assert(isInLoopThread()); }

private:
	vector<callback> _functors;
};