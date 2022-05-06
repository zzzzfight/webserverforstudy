#pragma once

//深入理解C++对象模型 父类或成员对象无拷贝构造或者赋值构造则 本类无拷贝构造 赋值构造同

class Nocopyable
{
	public:
	Nocopyable()=default;
// private:
	Nocopyable(const Nocopyable&)=delete;
	void operator=(const Nocopyable&)=delete;
};
