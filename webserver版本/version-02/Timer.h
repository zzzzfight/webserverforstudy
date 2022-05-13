#pragma once
#include "./base/MutexLock.h"
#include <queue>
#include <memory>
#include <vector>
#include <sys/time.h>
#include <iostream>
#include <deque>
// #include "HttpData.h"
using namespace std;
// #include<d>
class HttpData;
// class Channel;
// class TimerComp;
class TimerNode
{
public:
	TimerNode(std::shared_ptr<HttpData> &ptr, size_t timeout);
	// 	: _timeout(timeout), _httpdata(ptr), _deleted(false)
	// {
	// 	// struct timeval now;
	// 	// gettimeofday(&now, NULL);
	// 	// // 以毫秒计
	// 	// _expiredtime = (((now.tv_sec % 10000) * 1000) + (now.tv_usec / 1000)) + timeout;
	// }
	TimerNode();
	~TimerNode();
	size_t GetExpiredTime();

	bool IsDeleted();
	void SetDeleted();
	bool IsVaild();
	// {
	// 	struct timeval now;
	// 	gettimeofday(&now, NULL);
	// 	size_t temp = (((now.tv_sec % 10000) * 1000) + (now.tv_usec / 1000));
	// 	if (temp < _expiredtime)
	// 	{
	// 		std::cout << "delete timer" << std::endl;
	// 		return true;
	// 	}
	// 	else
	// 	{
	// 		this->SetDeleted();
	// 		return false;
	// 	}
	// }
	void ClearReq();

	// private:
	size_t _timeout;
	size_t _expiredtime;
	std::shared_ptr<HttpData> _httpdata;
	bool _deleted = false;
};

class TimerComp
{
public:
	bool operator()(std::shared_ptr<TimerNode> &a, std::shared_ptr<TimerNode> &b) const
	{
		return a->GetExpiredTime() > b->GetExpiredTime();
	}
};
class Timer
{
public:
	Timer();
	~Timer();
	void add_timer(std::shared_ptr<HttpData> ptr, size_t timeout);
	// {
	// 	TimerNode *newnode = new TimerNode(ptr, timeout);
	// 	// _queue.push(newnode);
	// 	_queue.push(newnode);
	// 	// MutexLockGuard lock(_mutex);
	// 	// _queue.push_back();
	// }
	void HandleExpiredEvent();
	priority_queue<shared_ptr<TimerNode>, deque<shared_ptr<TimerNode>>, TimerComp> _queue;
	// MutexLock _mutex;
};