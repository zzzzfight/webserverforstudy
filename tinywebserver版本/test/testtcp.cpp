#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <exception>
#include <memory>
#include"../timer/list_timer.h"




class WebServer{
public:
	WebServer(){

	}


	void EventLoop(){
		
	}

private:
	sort_time_lst TimerList;
	void dealclient(){

	}
};



using namespace std;

class TcpData
{
public:
	TcpData()
	{
		memset(&sockaddr, 0, sizeof(sockaddr));
		socklen = sizeof(sockaddr);
	}
	sockaddr_in sockaddr;
	socklen_t socklen;
};

void addfd(int epollfd, int fd, bool one_shot, int TRIGMode)
{
	epoll_event event;
	event.data.fd = fd;

	if (1 == TRIGMode)
		event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
	else
		event.events = EPOLLIN | EPOLLRDHUP;

	if (one_shot)
		event.events |= EPOLLONESHOT;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
	setnonblocking(fd);
}

//设置非阻塞
int setnonblocking(int fd)
{
	int old_option = fcntl(fd, F_GETFL);
	int new_option = old_option | O_NONBLOCK;
	fcntl(fd, F_SETFL, new_option);
	return old_option;
}

int main()
{
	int sockfd;
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	cout << sockfd << endl;
	sockaddr_in address;
	const char *ip = "172.27.127.226";
	address.sin_family = AF_INET;
	address.sin_port = htons(9006);
	inet_pton(AF_INET, ip, &address.sin_addr);

	int ret = bind(sockfd, (struct sockaddr *)(&address), sizeof(address));
	ret = listen(sockfd, 5);
	cout << ret << endl;

	char buffer[1024];
	memset(buffer, '\0', 1024);

	sockaddr_in recvaddr;
	memset(&recvaddr, 0, sizeof(sockaddr_in));
	socklen_t clent_addsize = sizeof(recvaddr);

	int connfd = accept(sockfd, (sockaddr *)&recvaddr, &clent_addsize);

	epoll_event events[1024];
	int epollfd = epoll_create(5);
	addfd(epollfd, sockfd, false, 0);

	bool stopevent = false;
	while (!stopevent)
	{
		int num = epoll_wait(epollfd, events, 1024, -1);
		if (num < 0)
		{
			throw std::exception();
		}

		for (int i = 0; i < num; i++)
		{
			int tempfd = events[i].data.fd;
			if (tempfd == sockfd)
			{
				auto data = make_shared<TcpData>();
				int clientfd = accept(tempfd, (sockaddr *)&(data->sockaddr), &(data->socklen));
				addfd(epollfd, clientfd, false, 0);
				
			}
		}
	}

	ret = recv(connfd, buffer, 1024, 0);
	// cout<<"recv 0";
	// exit(0);
	cout << ret << endl;
	cout << buffer << endl;

	send(connfd, buffer, 1024, 0);
}
