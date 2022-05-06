#ifndef _CHANNEL_
#define _CHANNEL_
#include <iostream>
#include <functional>
#include <memory>

// #include "EventLoop.h"
// #include "HttpData.h"

using namespace std;
class EventLoop;
// class Channel;
class HttpData;

class Channel
{
	using callback = typename std::function<void()>;

public:
	Channel(EventLoop *loop);
	Channel(EventLoop *loop, int fd);

	// usin

	void SetFd(int fd);
	int GetFd();

	void SetEvents(unsigned int evnt);
	void SetREvents(unsigned int evnt);
	// void SetEvents(unsigned int evnt);
	unsigned int GetEvents();
	unsigned int GetREvents();

	void SetReadHandler(callback &&cb);
	void SetWriteHandler(callback &&cb);
	void SetConnHandler(callback &&cb);

	// setHolder(std::shared_ptr<HttpData>& http);

	bool EqualAndUpdateLastEvents()
	{
		bool ret = (LastEvents == Events);
		LastEvents = Events;
		return ret;
	}

	// callback EventHandler;
	void EventHandler();
	void SetHolder(std::shared_ptr<HttpData> http);

private:
	callback ReadHandler;
	callback WriteHandler;
	callback ErrorHandler;
	callback ConnHanndler;

private:
	int _mfd;
	int _state;
	unsigned int REvents;
	unsigned int Events;
	unsigned int LastEvents;
	EventLoop *_baseloop;
	std::weak_ptr<HttpData> _http;
	// std::weak_ptr<HttpData>holder_;
};

#endif