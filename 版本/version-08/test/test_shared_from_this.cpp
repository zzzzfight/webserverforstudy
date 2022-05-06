#include <memory>
#include <iostream>
#include <functional>

using namespace std;

using callback = function<void()>;
class A;
class B
{
public:
	~B()
	{
		cout << a.use_count();
	}
	callback func1;
	callback func2;
	weak_ptr<A> a;
};

class A : public enable_shared_from_this<A>
{
public:
	A() : b(new B()) {}
	~A()
	{
		cout << shared_from_this().use_count() << endl;
	}
	void Init()
	{
		b->func1 = bind(&A::func1, shared_from_this());
		b->func2 = bind(&A::func2, shared_from_this());
	}
	void func1()
	{
		cout << 1 << endl;
	}
	void func2()
	{
		cout << 2 << endl;
	}
	shared_ptr<B> b;
};

int main()
{
	shared_ptr<A> a(new A());
	{
		cout << a.use_count() << endl;

		a->Init();
		cout << a.use_count() << endl;
	}
}