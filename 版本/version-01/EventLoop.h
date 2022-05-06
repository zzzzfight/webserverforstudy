#include <sys/socket.h>
#include <sys/epoll.h>
#include <memory>
#include "Epoll.h"
#include "Channel.h"
using namespace std;
// using SP_Channel = typename shared_ptr<Channel>;
class EventLoop
{
public:
	EventLoop() : _epoll(new Epoll()) {}
	void loop(void);
	
private:
	int Sockfd;
	const static int MaxFd = 1024;
	// epoll_event Events[MaxFd];
	shared_ptr<Epoll> _epoll;
};

