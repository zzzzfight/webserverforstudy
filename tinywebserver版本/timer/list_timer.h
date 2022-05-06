// #pragma once
#include<arpa/inet.h>
#include<time.h>
#include<iostream>

#define BUFFER_SIZE 64
class util_timer;

//用户数据类
class client_data{
public:
	sockaddr_in address;
	int sockfd;
	char buf[BUFFER_SIZE];
	util_timer* timer;
};

//计时器节点类
class util_timer
{
public:
	util_timer():prev(nullptr), next(nullptr){}

	~util_timer()=default;

	//绝对时间
	// int expire;
	time_t expire;

	//定时任务函数
	void (*cb_func)(client_data*);
	
	client_data* user_data;
	util_timer* prev;
	util_timer* next;
};

//计时器链表类
class sort_time_lst
{
private:
	util_timer* head;
	util_timer* tail;
	void add_timer(util_timer*, util_timer*);
public:
	sort_time_lst():head(nullptr), tail(nullptr){}

	~sort_time_lst();

	//链表新增计时器
	bool add_timer(util_timer*);

	//链表删除计时器
	bool del_timer(util_timer*);

	//链表调整计时器
	bool adjust_timer(util_timer*);

	//处理回调函数
	void tick();
	//test
	// void cout1(){
	// 	util_timer* temp = head;
	// 	while(temp!=nullptr){
	// 		std::cout<<temp->expire<<std::endl;
	// 		temp = temp->next;
	// 	}
	// }

};

sort_time_lst::~sort_time_lst()
{
	util_timer* temp;
	while(head){
		temp = head->next;
		delete head;
		head = temp;
	}
}

bool sort_time_lst::add_timer(util_timer* timer){
	if(!timer){
		return false;
	}
	else if(!head){
		head = tail = timer;
	}
	else if(timer->expire<head->expire){
		timer->next = head;
		head->prev = timer;
		head = timer;
	}
	else{
		add_timer(timer, head);
	}
	return true;
}

void sort_time_lst::add_timer(util_timer* timer, util_timer* head){
	util_timer* temp;
	while(head->next&&timer->expire>head->next->expire){
		// temp = head;
		// head = head->next;
		head = head->next;
	}
	if(!head->next){
		tail->next = timer;
		timer->prev = tail;
		tail = timer;
		timer->next = nullptr;
	}else{
		timer->next = head->next->prev;
		timer->next->prev = timer;
		timer->prev = head;
		head->next = timer;
	}
	return;
}

bool sort_time_lst::del_timer(util_timer* timer){
	if(!timer){
		return false;
	}
	if(timer == head && timer == tail){
		delete timer;
		head = tail = nullptr;
		return true;
	}
	if(timer == head){
		head->next->prev = nullptr;
		head = head->next;
		delete timer;
		return true;
	}
	if(timer == tail){
		tail->prev->next = nullptr;
		tail = tail->prev;
		delete timer;
		return true;
	}
	timer->prev->next = timer->next;
	timer->next->prev = timer->prev;
	delete timer;
	return true;	
}

bool sort_time_lst::adjust_timer(util_timer* timer)
{
	if(!timer)
	{
		return false;
	}
	util_timer* tmp = timer->next;
/*如果被调整的目标定时器处在链表尾部，或者该定时器新的超时值仍然小于其下一个
时器的超时值，则不用调整*/
	if(!tmp||(timer->expire < tmp->expire))
	{
		return true;
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

void sort_time_lst::tick()
{
	if(!head)
	{
		return;
	}
	std::cout<<"timer tick"<<std::endl;
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
		tmp = head;
	}
}

// int main(){
// 	util_timer* t1 = new util_timer();
// 	util_timer* t2 = new util_timer();
// 	util_timer* t3 = new util_timer();
// 	t1->expire = 10;
// 	t2->expire = 5;
// 	t3->expire = 12;
// 	sort_time_lst* ll = new sort_time_lst();
// 	ll->add_timer(t1);
// 	ll->cout1();
// 	std::cout<<"-----------"<<std::endl;
// 	ll->add_timer(t2);
// 	ll->cout1();
// 	std::cout<<"-----------"<<std::endl;
// 	ll->add_timer(t3);
// 	ll->cout1();
// 	std::cout<<"-----------"<<std::endl;
// 	ll->del_timer(t1);
// 	ll->cout1();
// 	std::cout<<"-----------"<<std::endl;
// 	ll->del_timer(t3);
// 	ll->cout1();
// 	std::cout<<"-----------"<<std::endl;
// 	ll->del_timer(t2);
// 	ll->cout1();
// 	// delete t1;
// 	// delete t2;
// 	// delete t3;
// 	// delete ll;
// }