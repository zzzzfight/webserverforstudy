#include "Timer.h"
#include <sys/time.h>
#include <memory>
#include <iostream>
#include <queue>
// #include<>
class Channel;
using namespace std;
TimerNode::TimerNode(std::shared_ptr<Channel> &ptr, size_t timeout)
	: _httpdata(ptr), _deleted(false), _timeout(timeout)
{
	struct timeval now;
	gettimeofday(&now, NULL);
	_expiredtime = (((now.tv_sec % 10000) * 1000) + (now.tv_usec / 1000)) + timeout;
}
TimerNode::~TimerNode() {}

size_t TimerNode::GetExpiredTime()
{
	return _expiredtime;
}

bool TimerNode::IsDeleted()
{
	return _deleted;
}
void TimerNode::SetDeleted()
{
	_deleted = true;
}
bool TimerNode::IsVaild()
{
	struct timeval now;
	gettimeofday(&now, NULL);
	size_t temp = (((now.tv_sec % 10000) * 1000) + (now.tv_usec / 1000));
	if (temp <= _expiredtime)
	{
		return true;
	}
	this->SetDeleted();
	return false;
}

void TimerNode::ClearReq() {}

Timer::Timer() {}
Timer::~Timer() {}

void Timer::add_timer(shared_ptr<Channel> ptr, size_t timeout)
{
	shared_ptr<TimerNode> newnode(new TimerNode(ptr, timeout));
	// _queue.
	cout << "push new node\n";
	_queue.push(newnode);
}

void Timer::HandleExpiredEvent()
{
	while (!_queue.empty())
	{
		shared_ptr<TimerNode> node = _queue.top();
		if (node->IsDeleted())
		{
			cout << "pop a node\n";
			cout << node->GetExpiredTime() << endl;
			_queue.pop();
		}
		else if (node->IsVaild() == false)
		{
			cout << "pop a node\n";
			cout << node->GetExpiredTime();
			_queue.pop();
		}
		else
		{
			break;
		}
	}
}
