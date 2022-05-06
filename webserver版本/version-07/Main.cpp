#include "EventLoop.h"

#include "Server.h"
#include <iostream>
using namespace std;

int main()
{
	// EventLoop loop;
	// EventLoop *loop = new EventLoop();
	EventLoop loop;
	cout << &loop << endl;
	Server myserver(&loop,4);
	myserver.ServerStart();
	loop.Looping();
}