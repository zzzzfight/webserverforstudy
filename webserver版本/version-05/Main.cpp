#include"EventLoop.h"

#include"Server.h"


int main(){
	EventLoop loop;
	Server myserver(&loop);
	myserver.ServerStart();
	loop.Looping();


}