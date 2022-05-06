// epoll是一个管理epoll操作的类，同时是CHANNEL和fd的中间层，负责epoll操作以及 fd与channel的转换

#include <sys/socket.h>
#include <sys/epoll.h>
#include <memory>
#include "Channel.h"
#include <vector>
#include <assert.h>

using SP_Channel = typename std::shared_ptr<Channel>;
class Channel;
class Epoll
{
public:
	// typedef std::shared_ptr<Channel> SP_Channel;

	Epoll();

	//返回已触发的事件
	std::vector<SP_Channel> Polling(int EventNums);
	void epoll_add(SP_Channel request, int timeout);
	void epoll_del(SP_Channel request);
	void epoll_mod(SP_Channel request, int timeout);

	vector<SP_Channel> poll()
	{
		int nums = epoll_wait(EpollFd, &*Events.begin(), MAXFD, EPOLLTIMEWAIT);
		vector<SP_Channel> ret;
		for (int i = 0; i < nums; i++)
		{
			int tempfd = Events[i].data.fd;
			// if(Fd2Chan[])
			SP_Channel tempret = Fd2Chan[tempfd];
			if (!tempret)
			{
			}
			else
			{
				tempret->set_Revent(Events[i].events);
				tempret->set_Event(0);
			}
			ret.push_back(tempret);
		}
		return ret;
	}

private:
	// vector<SP_Channel>list;
	// int EventNums;				// epoll_wait()返回的就绪事件数量
	int EpollFd;					 //监听的epollfd
	std::vector<epoll_event> Events; //事件数组

	std::vector<SP_Channel> Fd2Chan;
	const unsigned int EPOLLTIMEWAIT = 10000;
	const unsigned int MAXFD = 10000;
};