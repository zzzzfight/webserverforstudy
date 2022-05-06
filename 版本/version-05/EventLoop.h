#pragma once
#include "Epoll.h"

#include <memory>

#include "Channel.h"
#include <functional>
#include <vector>

class EventLoop
{
	using Functor = std::function<void()>;

public:
	EventLoop();

	void Looping();

	void AddtoPoll(std::shared_ptr<Channel> chn, int timeout = 0);
	// void AddtoLoop
	void UpdatetoPoll(std::shared_ptr<Channel> chn, int timeout = 0);

	void QueueInLoop(Functor cb);
	void DoPendingFunctor();

private:
	std::shared_ptr<Epoll> _epoll;
	// int
	bool _looping;

	std::vector<Functor> _functors;

	MutexLock _mutex;
};