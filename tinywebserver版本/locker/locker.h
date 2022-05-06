#ifndef LOCKER_H
#define LOCKER_H
#include<exception>
#include<pthread.h>
#include<semaphore.h>
/*封装信号量的类*/
// class locker;
// class cond;
class noncopyable
{
 private:
  noncopyable(const noncopyable&) = delete;
  void operator=(const noncopyable&) = delete;
 
 protected:
  noncopyable() = default;
  ~noncopyable() = default;
};


class sem
{
public:
/*创建并初始化信号量*/
	sem()
	{
		if(sem_init(&m_sem, 0, 0) != 0)
		{
		/*构造函数没有返回值，可以通过抛出异常来报告错误*/
			throw std::exception();
		}
	}
	sem(int num){
		if(sem_init(&m_sem, 0, num) != 0){
			throw std::exception();
		}
	}
	/*销毁信号量*/
	~sem()
	{
		sem_destroy(&m_sem);
	}
	/*等待信号量*/
	bool wait()
	{
		return sem_wait(&m_sem)==0;
	}
	/*增加信号量*/
	bool post()
	{
		return sem_post(&m_sem)==0;
	}
private:
	sem_t m_sem;
};


/*封装互斥锁的类*/
class locker:noncopyable
{
friend class cond;
public:
	/*创建并初始化互斥锁*/
	locker()
	{
		if(pthread_mutex_init(&m_mutex, NULL) != 0)
		{
			throw std::exception();
		}
	}
	/*销毁互斥锁*/
	// locker(const locker&)=delete;
	// locker &operator=(const locker&)=delete;
	~locker()
	{
		pthread_mutex_destroy(&m_mutex);
	}
	/*获取互斥锁*/
	bool lock()
	{
		return pthread_mutex_lock(&m_mutex) == 0;
	}
	/*释放互斥锁*/
	bool unlock()
	{
		return pthread_mutex_unlock(&m_mutex) == 0;
	}

// private:
	// pthread_mutex_t *get(){
	// 	return &m_mutex;
	// }
//	friend bool cond::wait(locker lock);
private:
	pthread_mutex_t m_mutex;
};

/*封装条件变量的类*/
class cond
{
	friend class locker;
public:
	/*创建并初始化条件变量*/
	cond()
	{
		if(pthread_cond_init(&m_cond, NULL) != 0)
		{
		/*构造函数中一旦出现问题，就应该立即释放已经成功分配了的资源*/
			// pthread_mutex_destroy(&m_mutex);
			throw std::exception();
		}
	}
/*销毁条件变量*/
	~cond()
	{
		// pthread_mutex_destroy(&m_mutex);
		pthread_cond_destroy(&m_cond);
	}

	// bool wait(pthread_mutex_t* mutex){
	// 	int ret = 0;
	// 	ret = pthread_cond_wait(&m_cond, m_mutex);
	// 	return ret == 0;
	// }

	bool wait(locker &lock){
		int ret = 0;
		ret = pthread_cond_wait(&m_cond,&(lock.m_mutex));
		return 0 == ret;
	}


/*等待条件变量*/
	// bool wait()
	// {
	// 	int ret = 0;
	// 	pthread_mutex_lock(&m_mutex);
	// 	ret=pthread_cond_wait(&m_cond, &m_mutex);
	// 	pthread_mutex_unlock(&m_mutex);
	// 	return ret == 0;
	// }
	/*唤醒等待条件变量的线程*/
	bool signal()
	{
		return pthread_cond_signal(&m_cond) == 0;
	}
	bool broadcast()
    {
        return pthread_cond_broadcast(&m_cond) == 0;
    }
private:
	pthread_cond_t m_cond;
};

#endif