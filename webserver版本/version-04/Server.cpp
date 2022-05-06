#include "Server.h"
#include "Util.h"
#include "EventLoop.h"
#include "HttpData.h"
#include <stdlib.h>
#include <functional>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include "Channel.h"

#include <assert.h>
// #include <errno.h>
// #include <string.h>
// #include <unistd.h>
#include <iostream>
using namespace std;
using callback = function<void()>;
// extern int errno;
Server::Server(EventLoop *loop, int threadnum, int port)
	: _baseloop(loop),
	  _threadnum(threadnum),
	  _port(port),
	  _oneloopperthreads(new EventLoopThreadPool(loop, threadnum)),
	  _listenfd(socket_bind_listen(_port)),
	  _acceptchannel(new Channel(loop, _listenfd))
{
	assert(_baseloop);
	// assert(_listenfd);
	assert(_oneloopperthreads);
	assert(_acceptchannel);
	assert(_listenfd > 0);
	cout << "_listenfd:" << _listenfd << endl;
	cout << _acceptchannel->GetFd() << endl;
	// _acceptchannel->SetFd(_listenfd);
	/*
	 * 根据unix环境高级编程一书，我们知道管道在读端关闭时，
	 * 写端继续写会引发SIGPIPE，而在网络（socket）连接的情况下，
	 * 如果接收端在发送端正在写的情况下关闭，
	 * 发送端也会触发SIGPIPE，如果不处理该信号，那么程序就会崩溃。
	 */
	handle_for_sigpipe();
	if (setSocketNonBlocking(_listenfd) < 0)
	{
		perror("set socket non block failed");
		abort();
	}
}

void Server::ServerStart()
{
	// _oneloopperthreads->ThreadPoolInit();
	_acceptchannel->SetEvents(EPOLLIN | EPOLLET);
	_acceptchannel->SetReadHandler(bind(&Server::HandNewConn, this));
	_acceptchannel->SetConnHandler(bind(&Server::HandThisConn, this));
	_baseloop->AddToEpoll(_acceptchannel, 0);
	started = true;
	// cout << _baseloop->_epoll->_events.size() << endl;
	cout <<"_fd2chan[4]" <<_baseloop->_epoll->_fd2chan[4] << endl;
	cout << "ServerStart" << endl;
}

void Server::HandThisConn()
{
	// _baseloop->UpdataLoop();
	_baseloop->UpdataEpoll(_acceptchannel, 0);
}
void Server::HandNewConn()
{
	cout << "handnewconn" << endl;
	sockaddr_in client_addr;
	memset(&client_addr, 0, sizeof(sockaddr_in));
	socklen_t client_addr_len = sizeof(socklen_t);
	int acceptfd = 0;
	while (acceptfd = accept(_listenfd, (sockaddr *)&client_addr, &client_addr_len))
	{

		EventLoop *curloop = _oneloopperthreads->GetNextPool();
		cout << curloop << endl;
		if (acceptfd >= MAXFDS)
		{
			close(acceptfd);
			continue;
		}

		//关闭NAGLE算法，减少短链接的时延
		setSocketNodelay(acceptfd);
		if (setSocketNonBlocking(acceptfd) < 0)
		{
			cout << "set non block failed!" << endl;
		}

		shared_ptr<HttpData> http(new HttpData(curloop, acceptfd));
		// http->
		http->GetChannel()->SetHolder(http);
		// curloop->QueueInLoop()
		// curloop->UpdataLoop();
		curloop->QueueInLoop(bind(&HttpData::NewEvent, http));
	}
	_acceptchannel->SetEvents(EPOLLIN | EPOLLET);
}
