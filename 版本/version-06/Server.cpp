#include "Server.h"
#include "Channel.h"
#include "./base/Util.h"
#include "HttpData.h"
#include <errno.h>
#include <memory>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
//
#include "EventLoop.h"
#include <functional>

#include "EventLoopThreadPool.h"
#include <assert.h>
#include <iostream>
using namespace std;
// #include<a>
Server::Server(EventLoop *loop, int threadnum, int port)
	: _baseloop(loop),
	  _port(port),
	  _listenfd(socket_bind_listen(port)),
	  _threadnum(threadnum),
	  _listenchann(new Channel(loop, _listenfd)),
	  _oneloopperthreads(new EventLoopThreadPool(_baseloop, threadnum))
{
	// std::cout << "Serverloop:" << loop << std::endl;
	// std::cout << "Server_baseloop:" << _baseloop << endl;
	assert(_baseloop);
	assert(_oneloopperthreads);
	assert(_listenchann);
	assert(_listenfd);
	std::cout << "Server_listenchann:" << _listenchann << std::endl;
	setSocketNonBlocking(_listenfd);
	setSocketNodelay(_listenfd);
	handle_for_sigpipe();
	// _listenchann->SetReadCb()
}

// CallBack SetReadCb(CallBack &&funcb);
// CallBack SetWriteCb(CallBack &&funcb);
// CallBack SetConnCb(CallBack &&funcb);

void Server::ServerStart()
{
	_oneloopperthreads->ThreadPoolInit();
	_listenchann->SetReadCb(std::bind(&Server::HandleNewConn, this));
	_listenchann->SetConnCb(std::bind(&Server::HandleThisConn, this));
	_listenchann->SetEvents(EPOLLIN | EPOLLET);
	_baseloop->AddtoPoll(_listenchann, 0);
	_started = true;
}

// void Server::HandleThisConn();
void Server::HandleThisConn()
{
	// _listenchann->SetEvents()
	_baseloop->UpdatetoPoll(_listenchann, 0);
}

void Server::HandleNewConn()
{
	// _listenchann->SetEvents(EPOLLIN | EPOLLET);
	_listenchann->SetEvents(0);
	while (true)
	{
		std::cout << "handnewconn" << std::endl;
		sockaddr_in clientaddr;
		socklen_t clientlen;
		int clientfd = accept(_listenfd, (sockaddr *)&clientaddr, &clientlen);
		if (clientfd == -1)
		{
			if (errno == EWOULDBLOCK)
				break;
			else if (errno == EINTR)
				continue;
		}
		else
		{
			setSocketNonBlocking(clientfd);
			setSocketNodelay(clientfd);
			EventLoop *curloop = _oneloopperthreads->GetNextPool();
			std::cout << "curloop:" << curloop << std::endl;
			assert(curloop);
			std::shared_ptr<HttpData> newhttp(new HttpData(curloop, clientfd));
			newhttp->GetChannel()->SetHolder(newhttp);

			// _loop->
			// _loop->AddtoPoll()
			cout << "handnewconn curloop:" << curloop->_threadid << endl;
			curloop->QueueInLoop(std::bind(&HttpData::NewConn, newhttp));
			// std::shared_ptr<Channel> newconn = new Channel(_loop, clientfd);
		}
	}
	// std::cout << "_listenchann:" << _listenchann << std::endl;
	// std::cout << &_listenchann->_events << std::endl;
	// std::cout << _listenchann->GetEvents() << std::endl;
}
