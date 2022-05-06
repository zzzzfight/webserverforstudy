#include "Channel.h"
#include <iostream>
using namespace std;
#include "sys/epoll.h"
int main()
{
	Channel chn(nullptr);
	unsigned int &events = chn.GetEvents();
	events = EPOLLIN | EPOLLOUT;
	cout << events << endl;
}