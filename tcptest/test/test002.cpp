#include <memory>
#include <iostream>
#include<functional>
using namespace std;
// class A
// {
// public:
// 	A(int b = 10) : b(b) {}
// 	int b;
// 	~A()
// 	{
// 		cout << "析构A" << endl;
// 	}
// };

// class B
// {
// public:
// 	B() : a(new A())
// 	{
// 	}
// 	~B()
// 	{
// 		cout << "析构B" << endl;
// 	}

// 	std::shared_ptr<A> a;

// private:
// };

class C
{
public:
	C() {}

	void per()
	{
		std::cout << "HELLOWORLD\n"
				  << endl;
	}
};

int main()
{
	// shared_ptr<A> p;
	// {
	// 	shared_ptr<B> b(new B());
	// 	p = b->a;
	// 	cout << p->b << endl;
	// }
	// cout << p->b << endl;
	C *c = nullptr;
	// C::per();
	auto f = bind(&C::per, c);
	f();
}