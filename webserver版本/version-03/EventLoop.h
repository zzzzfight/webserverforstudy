#pragma once

#include <memory>
#include <sys/epoll.h>
#include "../../tool/Nocopyable.h"
#include "CurrentThread.h"
#include "Epoll.h"
// #include<>
// using namespace std;
class EventLoop : Nocopyable
{
public:
	EventLoop();
	~EventLoop();
	void loop();
	void assertInLoopThread()
	{
		if (!isInLoopThread())
		{
			abortNoInLoopThread();
		}
	}

	bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }

private:
	void abortNoInLoopThread();

	bool looping_; //确保loop()只执行一次
	bool quit_;

	const pid_t threadId_;
	std::shared_ptr<Epoll> epoller_;
};