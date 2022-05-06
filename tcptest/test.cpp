// #include <sys/socket.h>
// #include <sys/epoll.h>
// #include <iostream>
// // #include<bits/sockaddr.h>
// #include <arpa/inet.h>
// #include <string.h>
// #include <fcntl.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include<queue>
// #include"./test/http_conn.h"
#include<iostream>
using namespace std;
priority_queue<int>res;
// static void addfd(int, int);
// static int setnonblocking(int);

// static void addfd(int epollfd, int fd)
// {
// 	epoll_event event;
// 	event.data.fd = fd;
// 	event.events = EPOLLIN|EPOLLET;
// 	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
// 	setnonblocking(fd);
// }
// static int setnonblocking(int fd){
// 	int old_option = fcntl(fd, F_GETFL);
// 	int new_option = old_option | O_NONBLOCK;
// 	fcntl(fd, F_SETFL, new_option);
// 	return old_option;
// }

// static int m_stop = 0;
bool comp(int a,int b){
	return a>b;
}
int main()
{
	priority_queue<int,vector<int>,decltype(&comp)>que(comp);
	vector<int>t{3,2,1};
	for(auto i:t)
	que.emplace(i);
// 	sockaddr_in server;
// 	memset(&server, 0, sizeof(server));
// 	server.sin_family = AF_INET;
// 	// server.sin_addr = argv[0];
	

// 	int port = 9006;

// 	server.sin_port = htons(port);

// 	char *ip = "172.18.39.172";

// 	inet_pton(AF_INET, ip, &server.sin_addr);
// 	// server.sin_addr.s_addr = htonl(INADDR_ANY);

// 	int sockfd = socket(PF_INET, SOCK_STREAM, 0);
// 	int ret = bind(sockfd, (sockaddr *)&server, sizeof(server));

// 	// int m_epollfd = epoll_create(5);
// 	// addfd(m_epollfd, sockfd);
// 	listen(sockfd, 5);
// 	// epoll_event events[1024];
// 	// while(!m_stop){
// 	// 	int num1 = epoll_wait(m_epollfd, events, 1024, -1);
// 	// 	for(int i = 0; i < num1; i++){
// 	// 		// events[i]
// 	// 	}
// 	// }



// 	sockaddr_in client;
// 	socklen_t client_address;
// 	printf("conneting test\n");


// //
// 	http_conn conn1;

// //
// 	int connfd = accept(sockfd, (sockaddr *)&client, &client_address);

// 	conn1.init(connfd);
// 	conn1.read_once();
// 	conn1.process_read();










// 	cout<<"method: "<<conn1.m_method<<endl;
// 	cout<<"url: "<<conn1.m_url<<endl;
// 	cout<<"version: "<<conn1.m_version<<endl;

// 	// fputs(buffer, stdout);
// 	fflush(stdout);



// 	// snprintf(buffer, 256,"")
// 	// snprintf(buffer, 1023, "HTTP/1.1 400 BAD_REQUESTION\r\nConnection: close\r\ncontent-length:%d\r\n\r\n", 256);
// 	// send(connfd, buffer, 1024, 0);
	


// 	close(connfd);
// 	close(sockfd);
}