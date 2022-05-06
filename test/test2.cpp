#include <memory>
#include<iostream>
using namespace std;
class A
{
public:
	A(int a = 3) : a(a)
	{
	}

	int a;
private:
};

shared_ptr<A>& getPtr(shared_ptr<A> &ptr)
{
	return ptr;
}

shared_ptr<A>& getPtr1(shared_ptr<A> &ptr)
{
	return ptr;
}

int main()
{
	shared_ptr<A> p(new A());
	auto p1 = getPtr(p);
	cout<<p1.use_count()<<endl;
	// cout<<p1->a<<endl;
	auto p2 = getPtr1(p1);
	cout<<p2.use_count()<<endl;
	
}