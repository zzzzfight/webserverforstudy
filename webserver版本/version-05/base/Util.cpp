#include "Util.h"
#include <sys/socket.h>
#include <arpa/inet.h>

#include <netinet/in.h>
#include <netinet/tcp.h>

#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include <errno.h>
#include <string.h>
#include <iostream>

// sigaction()
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
void setSocketNodelay(int fd)
{
	int enable = 1;
	setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (void *)&enable, sizeof(enable));
}
void setSocketNoLinger(int fd)
{
}
void shutDownWR(int fd)
{
}
int socket_bind_listen(int port)
{
	assert(port > 0 && port < 65536);
	int listenfd = socket(PF_INET, SOCK_STREAM, 0);

	assert(listenfd > 0);

	sockaddr_in listenaddr;
	bzero((char *)&listenaddr, sizeof(listenaddr));
	listenaddr.sin_port = htons(port);
	listenaddr.sin_family = AF_INET;
	listenaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int optval = 1;
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
	{
		close(listenfd);
		return -1;
	}

	int ret = bind(listenfd, (sockaddr *)&listenaddr, sizeof(listenaddr));

	// char *msg =  strerror(errno);
	// perror(msg);
	assert(ret == 0);

	if (listen(listenfd, 1024) == -1)
	{
		close(listenfd);
		return -1;
	}

	// 无效监听描述符
	if (listenfd == -1)
	{
		close(listenfd);
		return -1;
	}
	return listenfd;

	// setSocketNonBlocking()
}
