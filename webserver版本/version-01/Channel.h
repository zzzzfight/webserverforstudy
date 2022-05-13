#pragma once

#include <functional>
#include <memory>
//状态/
#include <iostream>
using namespace std;

typedef std::function<void()> CallBack;
class HttpData;
class EventLoop;
class Channel
{
	// using CallBack = typename std::function<void()>;

public:
	Channel(EventLoop *loop);
	Channel(EventLoop *loop, int fd);
	~Channel()
	{
		cout << "析构Channel" << endl;
	}
	//回调函数
	CallBack DealWithRead;
	CallBack DealWithWrite;
	CallBack DealWithConn;

	void SetReadCb(CallBack &&funcb);
	void SetWriteCb(CallBack &&funcb);
	void SetConnCb(CallBack &&funcb);

	void SetEvents(unsigned int env);
	unsigned int &GetEvents(void);

	void SetREvents(unsigned int env);
	unsigned int &GetREvents(void);

	int GetFd();
	void SetFd(int fd);

	void handleEvents();
	void SetHolder(std::shared_ptr<HttpData> &httpdata) { _httpdata = httpdata; }
	std::shared_ptr<HttpData> GetHolder()
	{
		std::shared_ptr<HttpData> ret(_httpdata.lock());
		return ret;
	}

	// private:
	EventLoop *_loop;

	std::weak_ptr<HttpData> _httpdata;
	int _mfd;

	unsigned int _events;
	unsigned int _revents;
	unsigned int _lastevents;
};