#include <vector>
#include <iostream>
#include <memory>
#include <queue>
using namespace std;

class A
{
public:
	A(int a) : a(a) {}
	int a;
};

struct comp
{
	bool operator()(shared_ptr<A> &a, shared_ptr<A> &b)
	{
		return a->a > b->a;
	}
};
int main()
{
	priority_queue<shared_ptr<A>, vector<shared_ptr<A>>, comp> _queue;
	shared_ptr<A> ptr1(new A(2));
	shared_ptr<A> ptr2(new A(5));
	_queue.push(ptr1);
	_queue.push(ptr2);
	cout << _queue.top()->a;
}