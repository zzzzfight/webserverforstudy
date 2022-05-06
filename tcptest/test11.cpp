#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
// #include<>
using namespace std;
extern int errno;
int main()
{
	sockaddr_in connaddr;
	connaddr.sin_family = AF_INET;
	connaddr.sin_port = htons(9006);
	char *ip = "172.22.169.251";
	inet_pton(AF_INET, ip, &connaddr.sin_addr);

	// connaddr.sin_addr=htonl()

	int connfd = socket(PF_INET, SOCK_STREAM, 0);
	assert(connfd > 0);
	// bind(connfd, (sockaddr *)&connaddr, sizeof(connaddr));
	connect(connfd, (sockaddr *)&connaddr, sizeof(sockaddr));
	// char *mesg = strerror(errno);
	// perror(mesg);
	// std::cout << mesg << std::endl;
	// char buffer[1024];
	char *buf1 = "helloworld";
	write(connfd, buf1, sizeof(buf1) + 1);
	char buf2[1024];
	mesg = strerror(errno);
	perror(mesg);
	read(connfd, buf2, 1024);
	cout << buf2 << endl;
	// assert(fd >= 0);

	// CONNECT
}