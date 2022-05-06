#pragma once
#include <memory>

class EventLoop;
class Channel;
class Server
{
public:
	Server(EventLoop *loop, int port = 9006);

	void ServerStart();

	void HandleNewConn();
	void HandleThisConn();

private:
private:
	EventLoop *_loop;

	// std::shared_ptr<Epoll> _epoll;
	std::shared_ptr<Channel> _listenchann;
	int _listenfd;
	int _port;
};
