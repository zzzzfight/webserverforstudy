#include <sys/epoll.h>
#include <sys/socket.h>
#include "Channel.h"
#include <vector>
#include <memory>
#include "Timer.h"
class HttpData;

#include "./base/MutexLock.h"

class Epoll
{
public:
	Epoll();
	~Epoll() {}

//增改删该对象下的产生的连接事件
	void Epoll_add(std::shared_ptr<Channel> chn, int timeout);
	void Epoll_mod(std::shared_ptr<Channel> chn, int timeout);
	void Epoll_del(std::shared_ptr<Channel> chn);
//事件轮询主体
	std::vector<std::shared_ptr<Channel>> Polling();

//从时间堆里面删除已超时的连接
	void HandleExpired();
private:
	int _epollfd;
	static const int MAXFDS = 1024;
	const static int MAXEVENTS = 1024;

//管理fd对应的事件channel
	std::shared_ptr<Channel> _fd2chan[MAXFDS];

//管理fd对应的http，保持http指针的持有直至主动释放
	std::shared_ptr<HttpData> _fd2http[MAXFDS];

//epoll返回的事件
	epoll_event _events[MAXEVENTS];
	Timer _timemanager;
};