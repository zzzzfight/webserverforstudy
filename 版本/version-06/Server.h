#pragma once
#include <memory>
// #include<memory>
// class EventLoopThreadPool;
#include "EventLoopThreadPool.h"
class EventLoop;
class Channel;
class Server
{
public:
	Server(EventLoop *loop, int threadnum = 8, int port = 9006);

	void ServerStart();

	void HandleNewConn();
	void HandleThisConn();

private:
	EventLoop *_baseloop;
	// std::shared_ptr<Epoll> _epoll;
	int _port;
	int _listenfd;
	int _threadnum;
	std::shared_ptr<Channel> _listenchann;
	std::unique_ptr<EventLoopThreadPool> _oneloopperthreads;
	bool _started = false;
	// start const int MAXFDS = 1024;
};
