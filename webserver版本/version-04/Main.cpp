// @Author Z
// @Email 3161349290@qq.com
#include "Acceptor.h"
#include "EventLoop.h"

int main()
{
	EventLoop mainLoop;
	Acceptor acceptor(&mainLoop);
	acceptor.start();
	mainLoop.loop();
}