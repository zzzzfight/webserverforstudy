/*

	一个非常简单的基于epoll的web服务器的实现
	使用epoll_wait监听http请求
	使用主从状态机对http协议进行解析并封装响应

*/

#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>

#include "httpdata.h"
// #inlcude<>
using namespace std;

void setnonblocking(int sock)
{
	int opts;
	opts = fcntl(sock, F_GETFL);
	if (opts < 0)
	{
		perror("fcntl(sock,GETFL)");
		exit(1);
	}
	opts = opts | O_NONBLOCK;
	if (fcntl(sock, F_SETFL, opts) < 0)
	{
		perror("fcntl(sock,SETFL,opts)");
		exit(1);
	}
}

ssize_t writen(int fd, void *buff, size_t n)
{
	size_t nleft = n;
	ssize_t nwritten = 0;
	ssize_t writeSum = 0;
	char *ptr = (char *)buff;
	while (nleft > 0)
	{
		if ((nwritten = write(fd, ptr, nleft)) <= 0)
		{
			if (nwritten < 0)
			{
				if (errno == EINTR)
				{
					nwritten = 0;
					continue;
				}
				else if (errno == EAGAIN)
				{
					return writeSum;
				}
				else
					return -1;
			}
		}
		writeSum += nwritten;
		nleft -= nwritten;
		ptr += nwritten;
	}
	return writeSum;
}

void handleError(int fd, int err_num, string short_msg)
{
	short_msg = " " + short_msg;
	char send_buff[4096];
	string body_buff, header_buff;
	body_buff += "<html><title>哎~出错了</title>";
	body_buff += "<body bgcolor=\"ffffff\">";
	body_buff += to_string(err_num) + short_msg;
	body_buff += "<hr><em> LinYa's Web Server</em>\n</body></html>";

	header_buff += "HTTP/1.1 " + to_string(err_num) + short_msg + "\r\n";
	header_buff += "Content-Type: text/html\r\n";
	header_buff += "Connection: Close\r\n";
	header_buff += "Content-Length: " + to_string(body_buff.size()) + "\r\n";
	header_buff += "Server: LinYa's Web Server\r\n";
	;
	header_buff += "\r\n";
	// 错误处理不考虑writen不完的情况
	sprintf(send_buff, "%s", header_buff.c_str());
	writen(fd, send_buff, strlen(send_buff));
	sprintf(send_buff, "%s", body_buff.c_str());
	writen(fd, send_buff, strlen(send_buff));
}

int main()
{

	// const char *ip = "9006";
	int port = atoi("9006");
	int ret = 0;
	sockaddr_in sockaddr1;
	bzero(&sockaddr1, sizeof(sockaddr1));
	sockaddr1.sin_family = AF_INET;
	sockaddr1.sin_addr.s_addr = htons(INADDR_ANY);
	// inet_pton(AF_INET, ip, &sockaddr1.sin_addr);
	sockaddr1.sin_port = htons(port);

	int sockfd = socket(PF_INET, SOCK_STREAM, 0);
	assert(sockfd >= 0);

	ret = bind(sockfd, (sockaddr *)&sockaddr1, sizeof(sockaddr1));
	assert(ret != -1);

	ret = listen(sockfd, 5);
	assert(ret != -1);

	setnonblocking(sockfd);
	int optval = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	epoll_event events[1024];
	int epollfd = epoll_create(5);
	assert(epollfd);

	epoll_event event1;
	event1.data.fd = sockfd;
	event1.events = EPOLLIN | EPOLLET;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &event1);

	string outbuffer;

	while (true)
	{
		int fdnum = epoll_wait(epollfd, events, 1024, -1);
		assert(fdnum >= 0);
		for (int i = 0; i < fdnum; i++)
		{
			int tempfd = events[i].data.fd;
			if (tempfd == sockfd)
			{
				sockaddr_in addr1;
				memset(&addr1, 0, sizeof(addr1));
				socklen_t addrlen1 = sizeof(addr1);
				int newfd = accept(tempfd, (sockaddr *)&addrlen1, &addrlen1);
				// epoll
				// setnonblocking(newfd);
				cout << "newfd" << endl;
				epoll_event event1;
				event1.events = EPOLLIN | EPOLLET;
				event1.data.fd = newfd;
				epoll_ctl(epollfd, EPOLL_CTL_ADD, newfd, &event1);
				setnonblocking(newfd);
			}
			else if (events[i].events & EPOLLIN)
			{
				char buffer[4096];
				memset(buffer, '\0', 4096);
				// recv(tempfd,buffer,)
				HttpData httpdata;
				int ret = recv(tempfd, buffer, 4096, 0);
				// httpdata.inbuffer = string(buffer);
				httpdata.inbuffer.append(buffer);
				cout << httpdata.inbuffer << "////////";
				httpdata.RequestLineAna();
				outbuffer.clear();
				outbuffer = httpdata.outbuffer;
				// std::cout << outbuffer << "////////";
				// if (ret)
				// {
				// 	buffer[ret] = '\0';
				// 	// cout << buffer << endl;
				// }

				// FILE *filefd = freopen("./tcpmesg.txt", "a+", stdout);
				// // cin >> buffer >>;
				// cout << buffer << "--------------" << endl;
				// fclose(filefd);
				// // int filefd = open("./tcpmesg.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
				// // int filefd
				// // ofstream output("./tcpmesg.txt", ofstream::out);
				// // write(filefd, buffer, 2048);
				epoll_event event1;
				event1.events = EPOLLOUT | EPOLLET;
				event1.data.fd = tempfd;
				epoll_ctl(epollfd, EPOLL_CTL_MOD, tempfd, &event1);
			}
			else if (events[i].events & EPOLLOUT)
			{
				// const char *buffer = "helloworld";

				// handleError(tempfd, 404, "NOT FOUND!");
				// send(tempfd, buffer, 1024, 0);
				if (outbuffer.size() > 0)
				{
					write(tempfd, outbuffer.c_str(), outbuffer.size());
				}
				else
				{
					handleError(tempfd, 404, "NOT FOUND");
				}
				epoll_event event1;
				event1.events = 0;
				event1.data.fd = tempfd;
				epoll_ctl(epollfd, EPOLL_CTL_DEL, tempfd, &event1);
				close(tempfd);
			}
			else if (events[i].events & EPOLLHUP)
			{
				epoll_event event1;
				event1.events = 0;
				event1.data.fd = tempfd;
				epoll_ctl(epollfd, EPOLL_CTL_DEL, tempfd, &event1);
				close(tempfd);
			}
		}
	}
}