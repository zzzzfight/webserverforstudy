#pragma once
#include <vector>
#include <memory>
#include<assert.h>
// #include<memory>
#include "./EventLoop.h"
#include "./Channel.h"
#include"EventLoopThreadPool.h"
#include <thread>
#include<string.h>
#include"../../tool/Util.h"

using namespace std;

pthread_once_t eltpOnceInit = PTHREAD_ONCE_INIT;
class Server
{
public:
	Server(EventLoop *baseloop, int ThreadNum, int Port = 9006)
		: _baseloop(baseloop),
		  _ThreadNum(ThreadNum),
		  _Port(Port),
		  _acceptchannel(new Channel(_baseloop)),
		  _listenfd(socket_bind_listen(_Port))
		//   _eltpstarted(false)
	// AcceptChannel()
	{
	}
	void EventLoopThreadPoolInit() {
		_threadpools = new EventLoopThreadPool(_ThreadNum);
		

		if(_threadpools){
			std::cout<<"eltp success"<<std::endl;
			_eltpstarted = true;
			_threadpools->ThreadPoolInit();
			// return true;
		}else{
			_eltpstarted = false;
			// return false;
		}

	}

	void ServerStart()
	{
		// _baseloop->
		pthread_once(&eltpOnceInit,EventLoopThreadPoolInit);
		assert(_eltpstarted);

	}
	void HandNewConn(){
		struct sockaddr_in client_addr;
		memset(&client_addr, 0, sizeof(struct sockaddr_in));
		// socklen_t client_addr_len = sizeof(struct sockaddr_in));
		socklen_t client_addr_len = sizeof(client_addr);
		int acceptfd = 0;
		while((acceptfd = accept(_listenfd, (struct sockaddr*)&client_addr),&client_addr_len))>0){
			EventLoop* loop = _threadpools->getNextLoop();
			if(acceptfd >= MAXFDS){
				close(accept_fd);
				continue;
			}
			if(setSocketNonBlocking(acceptfd)<0){
				std::cout<<"error"<<endl;
				return;
			}
			setSocketNodelay(acceptfd);
			shared_ptr<HttpData> req_info(new HttpData(loop,acceptfd));	
			req_info->GetChannel()->SetHolder(req_info);
			loop->queueInLoop(std::(&HttpData::newEvent,req_info));
		
		}
		_acceptchannel->setEvents(EPOLLIN|EPOLLET);
	}

private:
	EventLoop *_baseloop;
	shared_ptr<Channel> _acceptchannel;
	unique_ptr<EventLoopThreadPool> _threadpools;
	bool _eltpstarted;
	static const int MAXFDS = 1024;
	int _ThreadNum;
	int _listenfd;
};