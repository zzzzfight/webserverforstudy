#include "Timer.h"
#include <memory>
#include <iostream>
// #include <unistd.h>
#include <sys/unistd.h>
// #include<sys/>
// #include<>
#include <sys/time.h>
using namespace std;
class HttpData
{
public:
	HttpData(int a) : a(a) {}
	int a;
};

int main()
{
	Timer timers;
	shared_ptr<HttpData> ptr1(new HttpData(1));
	shared_ptr<HttpData> ptr2(new HttpData(2));
	shared_ptr<HttpData> ptr3(new HttpData(3));

	timers.add_timer(ptr1, 1000);
	timers.add_timer(ptr2, 5000);
	timers.add_timer(ptr3, 9000);
	while (1)
	{
		struct timeval now;
		gettimeofday(&now, NULL);
		size_t time = (((now.tv_sec % 10000) * 1000) + (now.tv_usec / 1000));
		cout << time << endl;
		sleep(1);
		cout << timers._queue.top()->_httpdata->a << endl;
		timers.HandleExpiredEvent();
	}
}