#include"Server.h"
#include"EventLoop.h"

// #include""
#include<iostream>
using namespace std;




int main(){
	EventLoop  mainloop;
	Server myserver(&mainloop, 8);
	myserver.ServerStart();
	mainloop.loop();
	

}