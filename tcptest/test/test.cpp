#include"http_conn.h"
#include<fcntl.h>
#include<unistd.h>
#include<iostream>
using namespace std;
#define MAX_BUFFER_SIZE 1024




int main(){
	int fp = open("./test.txt", O_RDONLY);
	http_conn conn1;
	// char buffer[1024];
	
	// fputs(buffer,stdout);
	// fflush(stdout);
	conn1.init();
	conn1.m_start_line = 0;
	conn1.m_read_idx = read(fp, conn1.m_read_buf, 1024);



	conn1.process_read();
	cout<<"----------------"<<endl;
	cout<<"method: "<<conn1.m_method<<endl;
	cout<<"url: "<<conn1.m_url<<endl;
	// cout<<endl;
	cout<<"version: "<<conn1.m_version<<endl;
	close(fp);
	exit(1);
}