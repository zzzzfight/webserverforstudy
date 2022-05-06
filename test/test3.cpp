#include <iostream>
#include <functional>
#include <pthread.h>
#include <memory>
using namespace std;

class A : public enable_shared_from_this<A>
{
public:
	A(){};
	int a;
	void pp()
	{
		{
			std::shared_ptr<A> guard(shared_from_this());
			cout << guard.use_count() << endl;
		}
		// use_count();
	}
};

int main()
{
	shared_ptr<A> temp(new A());
	cout << temp.use_count() << endl;
	temp->pp();
	cout << temp.use_count();
}
