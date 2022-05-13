#include <sys/socket.h>
#include <unordered_map>
#include <iostream>
#include <sys/errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/unistd.h>
#include <fcntl.h>

#include "../base/Util.h"
using namespace std;

// int setSocketNonBlocking(int fd)
// {
// 	int flag = fcntl(fd, F_GETFL, 0);
// 	if (flag == -1)
// 		return -1;

// 	flag |= O_NONBLOCK;
// 	if (fcntl(fd, F_SETFL, flag) == -1)
// 		return -1;
// 	return 0;
// }

// enum HTTP_MOTHED
// {
// 	GET,
// 	HEAD,
// 	POST
// };
// enum HTTP_VERSION
// {
// 	HTTP
// };
// enum HTTP_CODE
// {
// 	NO_REQUEST,
// 	GET_REQUEST,
// 	BAD_REQUEST,
// 	NO_RESOURCE,
// 	FILE_REQUEST,
// };
// enum LINE_STATUS
// {
// 	LINK_OK = 0,
// 	LINE_BAD,
// 	LINE_OPEN
// };
class HttpData
{
public:
	HttpData()
	{
		_inbuffer.clear();
	}

public:
	string _inbuffer;
	string _outbuffer;
	string _connection;
	// HTTP_MOTHED _mothed;
	string _mothed;
	string _httpversion;
	string _url;
	bool _keepalive;
};

int main()
{
	int sockfd = socket(PF_INET, SOCK_STREAM, 0);
	setSocketNodelay(sockfd);
	setSocketNoLinger(sockfd);
	sockaddr_in sockaddr1;
	memset(&sockaddr1, '\0', sizeof(sockaddr1));
	sockaddr1.sin_addr.s_addr = htonl(INADDR_ANY);
	sockaddr1.sin_family = AF_INET;
	sockaddr1.sin_port = htons(9006);
	int ret = bind(sockfd, (sockaddr *)&sockaddr1, sizeof(sockaddr1));
	ret = listen(sockfd, 5);
	sockaddr_in connaddr;
	socklen_t connaddr_len = sizeof(connaddr);
	memset(&connaddr, '\0', sizeof(connaddr));
	HttpData http;
	char *mesg = strerror(errno);
	perror(mesg);
	int connfd;
	while ((connfd = accept(sockfd, (sockaddr *)&connaddr, &connaddr_len)) > 0)
	{
		char *mesg = strerror(errno);
		perror(mesg);
		http._inbuffer.clear();

		char buf[1024];
		memset(buf, '\0', 1024);
		int readnum = read(connfd, buf, 1024);
		cout << readnum << endl;

		http._inbuffer.clear();
		http._inbuffer.append(string(buf, buf + 1024));

		cout << http._inbuffer << endl;
		int pos = http._inbuffer.find_first_of('\n');
		if (pos)
		{
			string headline = http._inbuffer.substr(0, pos);

			int backpos = headline.find_first_of(' ');
			string method = headline.substr(0, backpos);
			http._mothed = method;
			headline = headline.substr(backpos + 1);
			int urlpos = headline.find_first_of(' ');
			http._url = headline.substr(0, urlpos);

			headline = headline.substr(urlpos + 1);
			http._httpversion = headline;
		}
		cout << http._mothed << endl;
		cout << http._url << endl;
		cout << http._httpversion << endl;

		http._inbuffer.clear();

		close(connfd);
	}
	mesg = strerror(errno);
	perror(mesg);
	close(sockfd);
}