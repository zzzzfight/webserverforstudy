#include "EventLoop.h"
#include "./Channel.h"
#include <assert.h>
#include <memory>
// pthread_once_t
EventLoop::EventLoop() : threadId_(CurrentThread::tid()),
						 looping_(false),
						 epoller_(new Epoll())
{
}

void EventLoop::loop()
{
	assert(!looping_);
	quit_ = false;

	while (!quit_)
	{
		std::vector<std::shared_ptr<Channel>> ret;
		ret = epoller_->poll();
		for (auto &p : ret)
		{
			/*
				事件的处理
			*/
		}
	}
}