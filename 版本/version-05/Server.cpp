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
// #include<a>
Server::Server(EventLoop *loop, int port)
	: _loop(loop),
	  _port(port),
	  _listenfd(socket_bind_listen(port)),
	  _listenchann(new Channel(loop, _listenfd))
{
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
	_listenchann->SetReadCb(std::bind(&Server::HandleNewConn, this));
	_listenchann->SetConnCb(std::bind(&Server::HandleThisConn, this));
	_listenchann->SetEvents(EPOLLIN | EPOLLET);
	_loop->AddtoPoll(_listenchann, 0);
}

// void Server::HandleThisConn();
void Server::HandleThisConn()
{
	// _listenchann->SetEvents()
	_loop->UpdatetoPoll(_listenchann, 0);
}

void Server::HandleNewConn()
{
	while (true)
	{
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
			std::shared_ptr<HttpData> newhttp(new HttpData(_loop, clientfd));
			// _loop->
			// _loop->AddtoPoll()

			_loop->QueueInLoop(std::bind(&HttpData::NewConn, newhttp));
			// std::shared_ptr<Channel> newconn = new Channel(_loop, clientfd);
		}
	}
	_listenchann->SetEvents(EPOLLIN | EPOLLET);
}
