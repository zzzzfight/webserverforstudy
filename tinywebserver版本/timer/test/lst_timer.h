#ifndef LST_TIMER
#define LST_TIMER
#include<time.h>
#include<arpa/inet.h>
#include<cstdio>
//代码清单11-2  升序定时器链表
#define BUFFER_SIZE 64

class util_timer;/*前向声明*/
/*用户数据结构：客户端socket地址、socket文件描述符、读缓存和定时器*/

struct client_data
{
	sockaddr_in address;
	int sockfd;
	char buf[BUFFER_SIZE];
	util_timer* timer;
};
/*定时器类*/
class util_timer
{
public:
	util_timer():prev(NULL), next(NULL){}
public:
	time_t expire;/*任务的超时时间，这里使用绝对时间*/
	void(*cb_func)(client_data*);/*任务回调函数*/
/*回调函数处理的客户数据，由定时器的执行者传递给回调函数*/
	client_data* user_data;
	util_timer* prev;/*指向前一个定时器*/
	util_timer* next;/*指向下一个定时器*/
};
/*定时器链表。它是一个升序、双向链表，且带有头结点和尾节点*/

//链表类
class sort_timer_lst
{
public:
	sort_timer_lst():head(NULL), tail(NULL){}
/*链表被销毁时，删除其中所有的定时器*/
	~sort_timer_lst()
	{
		util_timer* tmp = head;
		while(tmp)
		{
			head = tmp->next;
			delete tmp;
			tmp = head;
		}
	}
/*将目标定时器timer添加到链表中*/
	void add_timer(util_timer* timer)
	{
		if(!timer)
		{
			return;
		}
		if(!head)
		{
			head = tail = timer;
			return;
		}
/*如果目标定时器的超时时间小于当前链表中所有定时器的超时时间，则把该定时器插
入链表头部，作为链表新的头节点。否则就需要调用重载函数
add_timer(util_timer*timer,util_timer*lst_head)，把它插入链表中合适的位
置，以保证链表的升序特性*/
		if(timer->expire < head->expire)
		{
			timer->next = head;
			head->prev = timer;
			head = timer;
			return;
		}
		add_timer(timer, head);
	}
/*当某个定时任务发生变化时，调整对应的定时器在链表中的位置。这个函数只考虑被
调整的定时器的超时时间延长的情况，即该定时器需要往链表的尾部移动*/
	void adjust_timer(util_timer* timer)
	{
		if(!timer)
		{
			return;
		}
		util_timer* tmp = timer->next;
/*如果被调整的目标定时器处在链表尾部，或者该定时器新的超时值仍然小于其下一个
定时器的超时值，则不用调整*/
		if(!tmp||(timer->expire < tmp->expire))
		{
			return;
		}
/*如果目标定时器是链表的头节点，则将该定时器从链表中取出并重新插入链表*/
		if(timer == head)
		{
			head = head->next;
			head->prev = NULL;
			timer->next = NULL;
			add_timer(timer, head);
		}
/*如果目标定时器不是链表的头节点，则将该定时器从链表中取出，然后插入其原来所
在位置之后的部分链表中*/
		else
		{
			timer->prev->next = timer->next;
			timer->next->prev=timer->prev;
			add_timer(timer, timer->next);
		}
	}
/*将目标定时器timer从链表中删除*/
	void del_timer(util_timer* timer)
	{
		if(!timer)
		{
			return;
		}
/*下面这个条件成立表示链表中只有一个定时器，即目标定时器*/
		if((timer == head)&&(timer == tail))
		{
			delete timer;
			head = NULL;
			tail = NULL;
			return;
		}
/*如果链表中至少有两个定时器，且目标定时器是链表的头结点，则将链表的头结点重
置为原头节点的下一个节点，然后删除目标定时器*/
		if(timer == head)
		{
			head = head->next;
			head->prev = NULL;
			delete timer;
			return;
		}
/*如果链表中至少有两个定时器，且目标定时器是链表的尾结点，则将链表的尾结点重
置为原尾节点的前一个节点，然后删除目标定时器*/
		if(timer == tail)
		{
			tail = tail->prev;
			tail->next = NULL;
			delete timer;
			return;
		}
/*如果目标定时器位于链表的中间，则把它前后的定时器串联起来，然后删除目标定时
器*/
		timer->prev->next = timer->next;
		timer->next->prev = timer->prev;
		delete timer;
	}
/*SIGALRM信号每次被触发就在其信号处理函数（如果使用统一事件源，则是主函数）
中执行一次tick函数，以处理链表上到期的任务*/
	void tick()
	{
		if(!head)
		{
			return;
		}
		printf("timer tick\n");
		time_t cur = time(NULL);/*获得系统当前的时间*/
		util_timer* tmp = head;
/*从头结点开始依次处理每个定时器，直到遇到一个尚未到期的定时器，这就是定时器
的核心逻辑*/
		while(tmp)
		{
/*因为每个定时器都使用绝对时间作为超时值，所以我们可以把定时器的超时值和系统
当前时间，比较以判断定时器是否到期*/
			if(cur < tmp->expire)
			{
				break;
			}
	/*调用定时器的回调函数，以执行定时任务*/
			tmp->cb_func(tmp->user_data);
	/*执行完定时器中的定时任务之后，就将它从链表中删除，并重置链表头结点*/
			head = tmp->next;
			if(head)
			{
				head->prev = NULL;
			}
			delete tmp;
			tmp=head;
		}
	}
private:
/*一个重载的辅助函数，它被公有的add_timer函数和adjust_timer函数调用。该
函数表示将目标定时器timer添加到节点lst_head之后的部分链表中*/
	void add_timer(util_timer* timer, util_timer* lst_head)
	{
		util_timer* prev = lst_head;
		util_timer* tmp = prev->next;
/*遍历lst_head节点之后的部分链表，直到找到一个超时时间大于目标定时器的超时
时间的节点，并将目标定时器插入该节点之前*/
		while(tmp)
		{
			if(timer->expire < tmp->expire)
			{
				prev->next = timer;
				timer->next = tmp;
				tmp->prev = timer;
				timer->prev = prev;
				break;
			}
			prev = tmp;
			tmp = tmp->next;
		}
/*如果遍历完lst_head节点之后的部分链表，仍未找到超时时间大于目标定时器的超
时时间的节点，则将目标定时器插入链表尾部，并把它设置为链表新的尾节点*/
		if(!tmp)
		{
			prev->next = timer;
			timer->prev = prev;
			timer->next = NULL;
			tail = timer;
		}
	}
private:
	util_timer* head;
	util_timer* tail;
};
#endif
//#include<netinet/in.h>
// #include<sys/socket.h>
// #include<sys/types.h>
// #include<string.h>
// #include<arpa/inet.h>
// #include<stdio.h>
// #include<stdlib.h>
// #include<unistd.h>
// #include<errno.h>
// #include<assert.h> 
// #include<fcntl.h>
// #include<sys/epoll.h>
// #include<pthread.h>

// #define MAX_EVENT_NUMBER 1024
// #define BUFFER_SIZE 1024

// struct fds
// {
// 	int epollfd;
// 	int sockfd;
// };
// int setnonblocking(int fd)
// {
// 	int old_option=fcntl(fd,F_GETFL);
// 	int new_option=old_option|O_NONBLOCK;
// 	fcntl(fd,F_SETFL,new_option);
// 	return old_option;
// }
// /*将fd上的EPOLLIN和EPOLLET事件注册到epollfd指示的epoll内核事件表中，参
// 数oneshot指定是否注册fd上的EPOLLONESHOT事件*/
// void addfd(int epollfd,int fd,bool oneshot)
// {
// 	epoll_event event;
// 	event.data.fd=fd;
// 	event.events=EPOLLIN|EPOLLET;
// 	if(oneshot)
// 	{
// 		event.events|=EPOLLONESHOT;
// 	}
// 	epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
// 	setnonblocking(fd);
// }
// /*重置fd上的事件。这样操作之后，尽管fd上的EPOLLONESHOT事件被注册，但是操
// 作系统仍然会触发fd上的EPOLLIN事件，且只触发一次*/
// void reset_oneshot(int epollfd, int fd)
// {
// 	epoll_event event;
// 	event.data.fd = fd;
// 	event.events = EPOLLIN|EPOLLET|EPOLLONESHOT;
// 	epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&event);
// }
// /*工作线程*/
// void*worker(void* arg)
// {
// 	int sockfd = ((fds*)arg)->sockfd;
// 	int epollfd = ((fds*)arg)->epollfd;
// 	printf("start new thread to receive data on fd:%d\n",sockfd);
// 	char buf[BUFFER_SIZE];
// 	memset(buf,'\0',BUFFER_SIZE);
// /*循环读取sockfd上的数据，直到遇到EAGAIN错误*/
// 	while(1)
// 	{
// 		int ret=recv(sockfd,buf,BUFFER_SIZE-1,0);
// 		if(ret==0)
// 		{
// 			close(sockfd);
// 			printf("foreiner closed the connection\n");
// 			break;
// 		}
// 		else if(ret < 0)
// 		{
// 			if(errno==EAGAIN)
// 			{
// 				reset_oneshot(epollfd,sockfd);
// 				printf("read later\n");
// 				break;
// 			}
// 		}
// 		else
// 		{
// 			printf("get content:%s\n",buf);
// 		/*休眠5s，模拟数据处理过程*/
// 			sleep(5);
// 		}
// 	}
// 	printf("end thread receiving data on fd:%d\n",sockfd);
// }

// int main(int argc,char*argv[])
// {
// 	if(argc<=2)
// 	{
// 		printf("usage:%s ip_address port_number\n",basename(argv[0]));
// 		return 1;
// 	}
// 	const char*ip=argv[1];
// 	int port=atoi(argv[2]);
// 	int ret=0;
// 	struct sockaddr_in address;
// 	bzero(&address,sizeof(address));
// 	address.sin_family=AF_INET;
// 	inet_pton(AF_INET,ip,&address.sin_addr);
// 	address.sin_port=htons(port);
// 	int listenfd=socket(PF_INET,SOCK_STREAM,0);
// 	assert(listenfd>=0);
// 	ret=bind(listenfd,(struct sockaddr*)&address,sizeof(address));
// 	assert(ret!=-1);
// 	ret=listen(listenfd,5);
// 	assert(ret!=-1);
// 	epoll_event events[MAX_EVENT_NUMBER];
// 	int epollfd=epoll_create(5);
// 	assert(epollfd!=-1);
// /*注意，监听socket listenfd上是不能注册EPOLLONESHOT事件的，否则应用程序
// 只能处理一个客户连接！因为后续的客户连接请求将不再触发listenfd上的EPOLLIN事件
// */
// 	addfd(epollfd,listenfd,false);
// 	while(1)
// 	{
// 		int ret=epoll_wait(epollfd,events,MAX_EVENT_NUMBER,-1);
// 		if(ret<0)
// 		{
// 			printf("epoll failure\n");
// 			break;
// 		}
// 		for(int i=0;i<ret;i++)
// 		{
// 			int sockfd=events[i].data.fd;
// 			if(sockfd==listenfd)
// 			{
// 				struct sockaddr_in client_address;
// 				socklen_t client_addrlength=sizeof(client_address);
// 				int connfd=accept(listenfd,(struct sockaddr*)&client_address, &client_addrlength);
// /*对每个非监听文件描述符都注册EPOLLONESHOT事件*/
// 				addfd(epollfd,connfd,true);
// 			}
// 			else if(events[i].events&EPOLLIN)
// 			{
// 				pthread_t thread;
// 				fds fds_for_new_worker;
// 				fds_for_new_worker.epollfd=epollfd;
// 				fds_for_new_worker.sockfd=sockfd;
// /*新启动一个工作线程为sockfd服务*/
// 				pthread_create(&thread,NULL,worker,(void*)&fds_for_new_worker);
// 			}
// 			else
// 			{
// 				printf("something else happened\n");
// 			}
// 		}
// 	}
// 	close(listenfd);
