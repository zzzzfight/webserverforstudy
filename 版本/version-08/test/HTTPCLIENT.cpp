#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../base/Util.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include <errno.h>
// #include<>

using namespace std;
int main()
{
	int sockfd = socket(PF_INET, SOCK_STREAM, 0);
	setSocketNodelay(sockfd);
	sockaddr_in connaddr;
	connaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	connaddr.sin_family = AF_INET;
	connaddr.sin_port = htons(9006);
	connect(sockfd, (sockaddr *)&connaddr, sizeof(connaddr));
	const char *filepath = "./http.txt";
	int filefd = open(filepath, O_RDONLY);
	// string buff;
	char buffer[4096];
	read(filefd, buffer, 4096);
	setSocketNonBlocking(sockfd);

	cout << buffer << endl;
	write(sockfd, buffer, 4096);
	char *mesg = strerror(errno);
	perror(mesg);
}