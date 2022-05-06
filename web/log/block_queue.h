#ifndef BLOCK_QUEUE_H
#define BLOCK_QUEUE_H

#include<iostream>
#include"../locker/locker1.h"
#include<queue>
#include<string.h>
#include<list>
using namespace std;

template<class datatype>
class blockQueue{
private:
	std::queue<datatype, std::list<datatype>>* que;
	long unsigned int q_size; //队列大小
	locker b_mutex;
	cond c_unempty; //队列非空
	cond c_unfull;  //队列非满
public:
	blockQueue(int size = 10);
	~blockQueue();
	bool queue_push(datatype buffer);
	bool queue_pop(datatype& buffer);
	bool queue_isfull(){
		return que->size() == q_size;
	}
	int que_size(){
		return que->size();
	}

	// bool queue_clear(datatype& buffer);
};

template<class datatype>
blockQueue<datatype>::blockQueue(int size){
	q_size = size;
	que = new std::queue<datatype, std::list<datatype>>();
}
template<class datatype>
blockQueue<datatype>::~blockQueue(){
	delete que;
}
//生产数据
template<class datatype>
bool blockQueue<datatype>::queue_push(datatype buffer){
	b_mutex.lock(); //加互斥锁
	// while(que->size() == q_size){
	// 	c_unempty.broadcast();
	// 	return false;
	// }
	while(que->size() == q_size){
		if(!c_unfull.wait(b_mutex))
			return false;
	}
	que->push(buffer);
	// cout<<"push"<<endl;
	// ++q_used;
	b_mutex.unlock();
	c_unempty.broadcast();	
	return true;
}	
//消费数据
template<class datatype>
bool blockQueue<datatype>::queue_pop(datatype &buffer){


	b_mutex.lock();
	while(que->size() == 0){

		if(!c_unempty.wait(b_mutex)){
			return false;
		}

	}
	
	buffer = que->front();
	que->pop();
	// cout<<"pop"<<endl;
	// --q_used;
	b_mutex.unlock();
	c_unfull.broadcast();
	return true;
}



// template<class datatype>
// bool blockQueue<datatype>::queue_clear(datatype &buffer){
// 	b_mutex.lock();
// 	while(que->size() == 0)
// 		return false;
// 	buffer = que->front();
// 	que->pop();
// 	// --q_used;
// 	b_mutex.unlock();
// 	return true;
// }




#endif
