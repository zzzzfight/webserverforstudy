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
#include <unistd.h>
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
	_listenchann->SetEvents(0);
	while (true)
	{
		std::cout << "handnewconn" << std::endl;
		sockaddr_in clientaddr;
		socklen_t clientlen;
		int clientfd = accept(_listenfd, (sockaddr *)&clientaddr, &clientlen);
		if (clientfd >= MAXFDS)
		{
			close(clientfd);
			continue;
		}
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
			newhttp->Init();
			// cout << newhttp.use_count() << endl;
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
	_listenchann->SetEvents(EPOLLIN | EPOLLET);
}

// void Server::HandleNewConn()
// {
// 	struct sockaddr_in client_addr;
// 	memset(&client_addr, 0, sizeof(struct sockaddr_in));
// 	socklen_t client_addr_len = sizeof(client_addr);
// 	int accept_fd = 0;
// 	while ((accept_fd = accept(_listenfd, (struct sockaddr *)&client_addr,
// 							   &client_addr_len)) > 0)
// 	{
// 		// EventLoop *loop = eventLoopThreadPool->getNextLoop();
// 		EventLoop *loop = _oneloopperthreads->GetNextPool();
// 		// LOG << "New connection from " << inet_ntoa(client_addr.sin_addr) << ":"
// 		//     << ntohs(client_addr.sin_port);
// 		// cout << "new connection" << endl;
// 		// cout << inet_ntoa(client_addr.sin_addr) << endl;
// 		// cout << ntohs(client_addr.sin_port) << endl;
// 		/*
// 		// TCP的保活机制默认是关闭的
// 		int optval = 0;
// 		socklen_t len_optval = 4;
// 		getsockopt(accept_fd, SOL_SOCKET,  SO_KEEPALIVE, &optval, &len_optval);
// 		cout << "optval ==" << optval << endl;
// 		*/
// 		// 限制服务器的最大并发连接数
// 		// if (accept_fd >= MAXFDS)
// 		// {
// 		// 	close(accept_fd);
// 		// 	continue;
// 		// }
// 		// 设为非阻塞模式
// 		if (setSocketNonBlocking(accept_fd) < 0)
// 		{
// 			// LOG << "Set non block failed!";
// 			perror("Set non block failed!");
// 			return;
// 		}

// 		setSocketNodelay(accept_fd);
// 		// setSocketNoLinger(accept_fd);

// 		std::shared_ptr<HttpData> newhttp(new HttpData(loop, accept_fd));
// 		newhttp->Init();
// 		// cout << newhttp.use_count() << endl;
// 		newhttp->GetChannel()->SetHolder(newhttp);

// 		// _loop->
// 		// _loop->AddtoPoll()
// 		cout << "handnewconn curloop:" << loop->_threadid << endl;
// 		loop->QueueInLoop(std::bind(&HttpData::NewConn, newhttp));
// 		// std::shared_ptr<Channel> newconn = new Channel(_loop, clientfd);
// 	}
// 	_listenchann->SetEvents(EPOLLIN | EPOLLET);
// }