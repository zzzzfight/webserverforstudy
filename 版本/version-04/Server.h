// #pragma once
#ifndef _SERVER_
#define _SERVER_
// #include <vector>
#include <memory>
// #include <assert.h>
// #include<memory>
// #include "./Channel.h"
// #include <thread>
// #include <string.h>
// #include "../../tool/Util.h"
#include "EventLoopThreadPool.h"
#include "Channel.h"
class EventLoop;
// #include "EventLoop.h"

using namespace std;

// pthread_once_t eltpOnceInit = PTHREAD_ONCE_INIT;

class Server
{
public:
	Server(EventLoop *loop = nullptr, int threadnum = 8, int poot = 9006);

	void ServerStart();

	void HandNewConn();
	void HandThisConn();
	// test
	// EventLoop *testgetnextpool()
	// {
	// 	EventLoop *temp = _oneloopperthreads->GetNextPool();
	// 	cout << temp->_pid << endl;
	// 	return temp;
	// }

private:
	unique_ptr<EventLoopThreadPool> _oneloopperthreads;
	EventLoop *_baseloop;
	int _threadnum;
	int _port;
	int _listenfd;

	std::shared_ptr<Channel> _acceptchannel;
	bool started = false;
	static const int MAXFDS = 1024;
};

#endif
