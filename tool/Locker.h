#pragma once
#include <pthread.h>
#include "./Nocopyable.h"

class MutexLock : Nocopyable
{
public:
	MutexLock()
	{
		pthread_mutex_init(&mutex, NULL);
	}
	~MutexLock()
	{
		pthread_mutex_destroy(&mutex);
	}
	bool lock()
	{
		return 0 == pthread_mutex_lock(&mutex);
	}
	bool unlock()
	{
		return 0 == pthread_mutex_unlock(&mutex);
	}
	friend class Condition;

private:
	pthread_mutex_t mutex;
};

class Condition : Nocopyable
{
public:
	Condition()
	{
		pthread_cond_init(&cond, NULL);
	}
	~Condition()
	{
		pthread_cond_destroy(&cond);
	}
	bool cond_wait(MutexLock &locker)
	{
		return 0 == pthread_cond_wait(&cond, &(locker.mutex));
	}
	bool cond_signal()
	{
		return 0 == pthread_cond_signal(&cond);
	}
	bool cond_broadcast()
	{
		return 0 == pthread_cond_broadcast(&cond);
	}

private:
	pthread_cond_t cond;
};

// class MutexLockGuard : Nocopyable
// {
// 	public:
// 	MutexLockGuard(MutexLock &){

// 	}
// };