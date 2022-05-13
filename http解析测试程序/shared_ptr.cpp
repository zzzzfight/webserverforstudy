#include <memory>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;

typedef function<void()> CB;
class B;
class A
{
public:
	A() { cout << "A 构造\n"; }
	~A() { cout << "A 析构\n"; }
	CB cb;
	shared_ptr<B> b_;
};
class B : public enable_shared_from_this<B>
{
public:
	B()
	{
		cout << "B 构造\n";
	}
	void init()
	{
		if (a_.lock())
		{
			a_.lock()->cb = bind(&B::run, shared_from_this());
		}
	}
	~B() { cout << "B 析构\n"; }
	void run()
	{
		cout << count << endl;
	}
	int count = 0;
	weak_ptr<A> a_;
};

void testttf()
{
	vector<shared_ptr<A>> vec;
	shared_ptr<A> test1(new A());
	shared_ptr<B> test2(new B());
	test2->a_ = test1;
	test1->b_ = test2;
	test2->init();

	// test->init();
	// test->a_->b_ = test;
	cout << test1.use_count() << endl;
	cout << test2.use_count() << endl;
	cout << "-------------------" << endl;
	// vec.push_back(test1);
	// cout << test2.use_count() << endl;
	// cout << "in {} :" << test2.use_count() << endl;
	// vec.pop_back();

	{
		shared_ptr<A> temp1 = test1;
		shared_ptr<B> temp2 = test2;
		cout << temp1.use_count() << endl;
		cout << temp2.use_count() << endl;
	}
	cout << "-----------------" << endl;
	cout << test1.use_count() << endl;
	cout << test2.use_count() << endl;
	cout << "------------" << endl;
	// test.reset();
	// test->a_.reset();
}
int main()
{
	testttf();
}
