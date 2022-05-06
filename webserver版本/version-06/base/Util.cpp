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
// #include <string>
// #include<String>
using namespace std;
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
	std::cout << "bind\n";
	char *msg = strerror(errno);
	perror(msg);
	// assert(ret == 0);

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

size_t readn(int fd, std::string &buffer)
{
	ssize_t nread = 0;
	ssize_t readSum = 0;
	const int MAX_BUFF = 1024;
	while (true)
	{
		char buff[MAX_BUFF];
		if ((nread = read(fd, buff, MAX_BUFF)) < 0)
		{
			if (errno == EINTR)
				continue;
			else if (errno == EAGAIN)
			{
				std::cout << buff << std::endl;
				break;
			}
			else
			{
				perror("read error");
				break;
			}
		}
		else if (nread == 0)
		{
			// printf("redsum = %d\n", readSum);
			// std::cout << buff << std::endl;

			break;
		}
		readSum += nread;
		buffer += std::string(buff, buff + nread);
	}
	// buffer += std::string(buff, buff + nread);
	// char *ptr = buff;
	// buffer.append(buff);
	return readSum;
}



// ssize_t readn(int fd, std::string &inBuffer, bool &zero)
// {
// 	ssize_t nread = 0;
// 	ssize_t readSum = 0;
// 	while (true)
// 	{
// 		char buff[MAX_BUFF];
// 		if ((nread = read(fd, buff, MAX_BUFF)) < 0)
// 		{
// 			if (errno == EINTR)
// 				continue;
// 			else if (errno == EAGAIN)
// 			{
// 				return readSum;
// 			}
// 			else
// 			{
// 				perror("read error");
// 				return -1;
// 			}
// 		}
// 		else if (nread == 0)
// 		{
// 			// printf("redsum = %d\n", readSum);
// 			zero = true;
// 			break;
// 		}
// 		// printf("before inBuffer.size() = %d\n", inBuffer.size());
// 		// printf("nread = %d\n", nread);
// 		readSum += nread;
// 		// buff += nread;
// 		inBuffer += std::string(buff, buff + nread);
// 		// printf("after inBuffer.size() = %d\n", inBuffer.size());
// 	}
// 	return readSum;
// }


