// #include"Server.h"
// #include"EventLoop.h"
// #include""

// test
// #include "Channel.h"
#include "EventLoopThread.h"
#include "Server.h"
#include "EventLoop.h"
#include <memory>
#include <iostream>
#include <vector>
//
#include <unistd.h>
using namespace std;

int main()
{
	EventLoop mainloop;
	cout << &mainloop << endl;
	Server myserver(&mainloop, 0);
	myserver.ServerStart();
	mainloop.Looping();
	// Channel a;

	// std::vector<std::shared_ptr<EventLoopThread>> _threads;
	// std::vector<EventLoop *> _loops;
	// for (int i = 0; i < 8; i++)
	// {
	// 	std::shared_ptr<EventLoopThread> _curthread(new EventLoopThread());
	// 	_threads.push_back(_curthread);
	// 	_loops.push_back(_curthread->ThreadStart());
	// }

	// Server server;
	// server.ServerStart();
	// for (int i = 0; i < 8; i++)
	// {
	// 	EventLoop *curloop;
	// 	if (curloop = server.testgetnextpool())
	// 	{
	// 		cout << "success for" << curloop->_pid << endl;
	// 	}
	// }
	// for (int i = 0; i < 3; i++)
	// {
	// 	sleep(1);
	// }
}