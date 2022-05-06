#include <thread>
#include <iostream>

#include <stdint.h>

using namespace std;

int main()
{
	int i = 1;
	if (__builtin_expect(i == 1, 0))
	{
		cout << "i==1" << endl;
	}
	else
	{
		cout << "i==0" << endl;
	}
}