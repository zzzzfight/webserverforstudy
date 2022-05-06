#include<iostream>
#include<vector>

#include<memory>

class A{
public:
	A()=default;
	int a = 10;
};

using namespace std;

//调用了拷贝构造，会增加
int main(){
	shared_ptr<A> ptr(new A);
	cout<<ptr.use_count()<<endl;
	vector<shared_ptr<A>>vec;
	vec.push_back(ptr);
	cout<<ptr.use_count()<<endl;
	{
	vector<shared_ptr<A>>vec1;
	vec1 = vec;
	cout<<ptr.use_count()<<endl;
	}
	cout<<ptr.use_count()<<endl;
}