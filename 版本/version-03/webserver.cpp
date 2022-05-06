#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <string.h>
// #include<epoll.h>
#include <sys/epoll.h>

using namespace std;

bool dealwithread(int fd);
bool dealwithwrite(int fd);
enum EVENTTYPE
{
	READISOK = 0,
	WRITEISOK
};

int main()
{
	int sockfd = socket(PF_INET, SOCK_STREAM, 0);
	sockaddr_in sockaddress;
	memset(&sockaddress, 0, sizeof(sockaddress));
	sockaddress.sin_port = htons(9006);
	sockaddress.sin_addr.s_addr = htonl(INADDR_ANY);
	sockaddress.sin_family = AF_INET;
	// bind(sockfd,sockaddr*())
	bind(sockfd, (sockaddr *)&sockaddress, sizeof(sockaddress));
	listen(sockfd, 5);
	// accept()

	int epollfd = epoll_create(1024);
	epoll_event events[1024];
	epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, events);

	int fdnumb = epoll_wait(epollfd, events, 1024, 0);

	while (true)
	{
		for (int i = 0; i < fdnumb; i++)
		{
			int tempfd = events[i].data.fd;
			if (tempfd == epollfd)
			{
				// if (epoll_ctl(epollfd, EPOLL_CTL_ADD, tempfd, 0) == -1)
				// {
				// 	cout << "err1" << endl;
				// 	exit(1);
				// }
				sockaddr_in connaddr;
				memset(&connaddr, 0, sizeof(connaddr));
				socklen_t connaddrlen;
				int connfd = accept(tempfd, (sockaddr *)&connaddr, &connaddrlen);
				if (epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, events) == -1)
				{
					cout << "err epoll ctl" << endl;
					exit(1);
				}
			}
			else if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))//对端关闭或者连接错误
			{
				epoll_ctl(epollfd,EPOLL_CTL_DEL,tempfd,events);
				close(tempfd);
			}
			else if (events[i].events & EPOLLIN)//文件描述符可读
			{
				dealwithread(tempfd);
			}
			else if (events[i].events & EPOLLOUT)//文件描述符可写
			{
				dealwithwrite(tempfd);
			}
		}
	}
}

bool dealwithread(int fd){
	
}
