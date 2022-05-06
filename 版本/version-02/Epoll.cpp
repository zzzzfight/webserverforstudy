#include <sys/epoll.h>
#include <vector>
#include "Epoll.h"

Epoll::Epoll()
{
	EpollFd = epoll_create(5);
	// EpollFd = epoll_create1(EPOLL_CLOEXEC);
	Fd2Chan.resize(MAXFD);
	assert(EpollFd > 0);
}

// std::vector<SP_Channel> Epoll::Polling(int EventNums)
// {
// 	// SP_Channel ret;
// 	EventNums = epoll_wait(EpollFd, &*Events.begin(), Events.size(), EPOLLTIMEWAIT);

// 	std::vector<SP_Channel> List;
// 	List.clear();
// 	for (int i = 0; i < EventNums; i++)
// 	{
// 		int TempFd = Events[i].data.fd;
// 		SP_Channel cur_rq = Fd2Chan[TempFd];
// 		if (!cur_rq)
// 		{
// 		}
// 		else
// 		{
// 			cur_rq->set_Event(Events[i].events);
// 			// cur_rq->set_Fd(TempFd);
// 		}
// 		List.push_back(cur_rq);
// 	}
// 	return List;
// }