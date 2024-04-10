#include <iostream>
using namespace std;

class A
{
public:
	A() : ptr(new int(10)) { std::cout << "普通构造" << std::endl; }
	A(A& tt) { this->b = tt.b; std::cout << "拷贝构造" << std::endl; }
	A(A&& cop) : ptr(cop.ptr) { this->b = cop.b;   cop.ptr = nullptr;  std::cout << "移动构造 " << std::endl; }
	A& operator=(A&& tt) noexcept{ std::cout << " 移动赋值" << std::endl; return *this; }
	A& operator=(const A& tt) { std::cout << " 拷贝赋值" << std::endl; return *this; }
	~A() { cout << "析构函数" << endl; }
	int b;
	int* ptr;
};

A getA()
{
	//第一种方式， 使用一个临时对象
	A a;
	a.b = 5;
	return a;
	//return A(); 第二种方式，直接返回。
}

int main()
{
	int n;
	cin >> n;
	A a = getA(); //会存在RVO优化
	cout << "b成员: " << a.b << endl;
	cout << "ptr指针成员: " << *a.ptr << endl;
	return 0;
}
