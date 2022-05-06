#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>
#include "Epoll.h"
#include "Util.h"
#include "EventLoop.h"
#include "Channel.h"
using namespace std;

class WebServer
{
public:
	WebServer(EventLoop *loop, int port, int ThreadNums = 8)
		: ListenFd(socket_bind_listen(port)),
		  ThreadNums(ThreadNums),
		  AcceptChann(new Channel(loop)),
		  _Loop(loop)
	{
		AcceptChann->set_Fd(ListenFd);
		//

		handle_for_sigpipe();
		setSocketNonBlocking(ListenFd);
	}

	~WebServer();

	void init()
	{
	}

	void start()
	{
		AcceptChann->set_Event(EPOLLIN | EPOLLET);

		// _Loop
		// AcceptChann
	}

private:
	// Epoll _Epoll;
	int ListenFd;
	int ThreadNums;
	shared_ptr<Channel> AcceptChann;
	EventLoop *_Loop;
};