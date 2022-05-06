#include <iostream>
#include <string>
#include <unistd.h>
#include <signal.h>

using namespace std;
class User_data
{
public:
	int a;
	std::string b;
};

class Timer_node
{
public:
	Timer_node(int rot, int slot) : next(nullptr), prev(nullptr), m_rot(rot), m_slot(slot) {}
	int m_slot;
	int m_rot;
	User_data m_data;
	Timer_node *next;
	Timer_node *prev;
	void *(*cb_func)(void *);
	// cb_func
};

class Timer_wheel
{
public:
	Timer_wheel() {}
	Timer_node *Timer_add(int timeout, User_data data, void *(*cb_func)(void *))
	{
		int rot = (timeout + cur) / N;
		int slot = (timeout + cur) % N;
		Timer_node *node = new Timer_node(rot, slot);
		node->m_data = data;
		// node->m_data = new User_data(data);
		node->cb_func = cb_func;
		Timer_insert(node);
		return node;
	}

	bool Timer_del(Timer_node *node)
	{
		if (!node)
		{
			return false;
		}
		if (slot[node->m_slot] == node)
		{
			slot[node->m_slot] = node->next;
			delete node;
			return true;
		}
		Timer_node *temp = slot[node->m_slot];
		while (temp)
		{
			if (temp == node)
			{
				if (temp->prev)
					temp->prev->next = node->next;
				if (temp->next)
					temp->next->prev = node->prev;
				delete node;
				return true;
			}
			temp = temp->next;
		}
		return false;
	}
	void Tick_run()
	{
		cur = (cur + cell) % N;
		Timer_node *temp = slot[cur];
		if (!temp)
		{
			return;
		}
		while (temp)
		{
			if (temp->m_rot != 0)
			{
				temp->m_rot--;
				// temp=temp->next;
			}
			else
			{
				temp->cb_func((void *)(&temp->m_data));
				// temp->prev->next = temp->next;
				// temp->next->prev = temp->prev;
				// delete temp;
				Timer_del(temp);
			}
			temp = temp->next;
		}
		return;
	}

private:
	void Timer_insert(Timer_node *node)
	{
		if (slot[node->m_slot] != nullptr)
		{
			node->next = slot[node->m_slot];
			node->next->prev = node;
			slot[node->m_slot] = node;
		}
		else
		{
			slot[node->m_slot] = node;
		}
	}
	static const int N = 60;
	static const int cell = 1;
	Timer_node *slot[N] = {0};
	int cur = 0;
};

Timer_wheel wheel;

void sig_alm(int signo)
{
	cout << "1 second left" << endl;
	wheel.Tick_run();
	alarm(1);
}
void *cb_func(void *data)
{
	// cout<<"this is one tick"<<endl;
	User_data *temp = (User_data *)data;
	cout << temp->a << endl;
}

int main()
{
	// signal(SIGALM)
	signal(SIGALRM, sig_alm);
	User_data data;
	data.a = 10;
	wheel.Timer_add(6, data, cb_func);
	wheel.Timer_add(19, data, cb_func);
	alarm(1);
	while (1)
	{
		// sleep(1);
		pause();
	}
}
