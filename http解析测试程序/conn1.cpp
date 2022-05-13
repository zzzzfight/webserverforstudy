/*
	2022-5-4
	回声客户端
*/
#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

// #include<>
using namespace std;
extern int errno;
int main()
{
	sockaddr_in connaddr;
	connaddr.sin_family = AF_INET;
	connaddr.sin_port = htons(9006);
	// const char *ip = "172.22.130.213";
	// inet_pton(AF_INET, ip, &connaddr.sin_addr);
	connaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	// connaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	// connaddr.sin_addr=htonl()

	int connfd = socket(PF_INET, SOCK_STREAM, 0);
	assert(connfd > 0);
	// bind(connfd, (sockaddr *)&connaddr, sizeof(connaddr));
	connect(connfd, (sockaddr *)&connaddr, sizeof(sockaddr));
	// char *mesg = strerror(errno);
	// perror(mesg);
	// std::cout << mesg << std::endl;
	// char buffer[1024];

	// char *buf1 = "helloworld";
	// int fd = open()

	// ofstream
	char buf1[1024];
	ifstream inf("./tcpmesg1.txt", ifstream::in);
	// read(inf,)
	inf >> buf1;
	inf.close();
	// open("/tcpmesg1.txt",ios::in,)
	write(connfd, buf1, 1024);
	cout << buf1 << endl;
	cout << "------------------\n";
	char buf2[1024];
	char *mesg = strerror(errno);
	perror(mesg);
	if (int ret = read(connfd, buf2, 1024))
	{
		cout << buf2 << endl;
	}
	cout << "------------------\n";
	// assert(fd >= 0);
	mesg = strerror(errno);
	perror(mesg);

	sleep(10);
	std::cout << "10's after\n";
	inf.open("./tcpmesg2.txt");
	char buf3[1024];
	inf >> buf3;
	cout << buf3 << endl;
	cout << "------------------\n";
	write(connfd, buf3, 1024);

	char buf4[1024];
	read(connfd, buf4, 1024);
	cout << buf4 << endl;
	// CONNECT
	cout << "------------------\n";
}