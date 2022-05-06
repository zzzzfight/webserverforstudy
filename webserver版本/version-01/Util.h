#pragma once
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include <fcntl.h>
using namespace std;
int socket_bind_listen(int port)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	int optval = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval,
				   sizeof(optval)) == -1)
	{
		close(sockfd);
		return -1;
	}

	sockaddr_in sockaddress;
	bzero((char *)&sockaddress, sizeof(sockaddr_in));
	sockaddress.sin_family = PF_INET;
	sockaddress.sin_port = htons((unsigned short)port);
	sockaddress.sin_addr.s_addr = htonl(INADDR_ANY);
	int ret = bind(sockfd, (sockaddr *)&sockaddress.sin_addr, sizeof(sockaddress));
	assert(ret > 0);
	ret = listen(sockfd, 2048);
	assert(ret > 0);
	return sockfd;
}

void handle_for_sigpipe()
{
	struct sigaction sa;
	memset(&sa, '\0', sizeof(sa));
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	if (sigaction(SIGPIPE, &sa, NULL))
		return;
}

int setSocketNonBlocking(int fd)
{
	int flag = fcntl(fd, F_GETFL, 0);
	if (flag == -1)
		return -1;

	flag |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flag) == -1)
		return -1;
	return 0;
}