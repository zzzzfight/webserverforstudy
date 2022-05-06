#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>
using namespace std;

int main()
{
	cout << (EPOLLET|EPOLLIN) << endl;
}