#include<iostream>
using namespace std;
class room {
public:
	int o;
	void prind() {
		cout << "ninini" << endl;
		space1;
		name1;
	}
protected:
	int name1;
private:
	int space1;
};
class ni :public room {
public:
	int o2;
	void d() {
		cout << "dddd" << endl;
		//public继承 ni类中不能使用space1可以使用space2可以调用name1与name2
		//protected继承 ni类中可以使用o，name1和name2，但在类外不能使用o，prind，name1和name2，相当于是room类中的public变为protected
		//private继承 room类中的public和protected变成ni类中的private但是还是不能访问ni类中的private成员
		// 如果想要访问父辈的private成员还需要通过函数继承的方式进行
		//因为有了继承的概念所以才引出了protected反式
		o;
		name1;
		;

	}
protected:
	int name2;
private:
	int space2;
};
//int main() {
	//ni a1;
	//room it = a1;当子类对象赋值给父类对象时，必须为public继承，并且是将子类中父类的值传给父类
	//room it = a1;//这里类型转化时没有产生临时对象，只是一种意义上的切割，引用与指针也一样
	//room& itt = a1;
	//room* itttt = &a1;


class c {
public:
	int a = 10;
	void print() {
		cout << a << endl;
	}
};
class d :public c {
public:
	int a = 20;
	void print() {
		cout << "aaa" << endl;
	}
};
//int main() {
//	d v;
//	v.print();//调用c中的a
//	int c = v.a;
//	cout << c<< endl;//会优先调用d中的a，如果想要调用父辈中的a，需要指定类域
//
class e {
public:
	// 因为拷贝构造函数本质也是要去调用你的基类的默认构造函数来初始化你的基类那部分对象的
	e(int i=0) {
		name = i;//默认构造
	}
	e(const e& v)
	{
		cout << "dddddd" << endl;

	}

	~e() {

	}
	void operator=(e&Ddddd) {//返回值没限制
		name = Ddddd.name;

	}
protected:
	int name;
};
class f :public e {
public:
	f(int h, int s, const char* sss)
		:gg(h)
		//,name(s),错误，父辈的成员不能在子类中进行初始化，必须调用父辈的构造函数进行初始化，如果想要调用必须实例化调用
		//这样可以初始化,显示调用
		,ss(sss)
	
	{
	}
	f(const f& l)
		//:e(l.name)
		//,gg(l.gg)
	{//这里没有默认构造函数那么我们写一个，虽然行，但是拷贝错了，拷贝的是默认值，而不是l中的name
		gg = l.gg;
		ss = l.ss;//可以这样，但是e中的成员没有被拷贝，使用的是默认值
		//name = l.name;这里是编译器规定不可以直接对父类成员进行初始化，必须用初始化进行显示调用
		//拷贝与初始化的顺序都是先父类后子类，然后按照成员定义变量来初始化的

	}
	e& operator=(f &yy) {
		//对于父辈的成员需要调用父辈的赋值，必须显示调用
		//operator=(yy)显示调用父辈的拷贝构造，但是这里与父类的赋值函数构成隐藏关系，会优先调用子类的赋值函数由此形成递归
		//所以这里需要指定类域
		e::operator=(yy);
		gg = yy.gg;
		ss = yy.ss;
		return *this;
	}
	~f() {
		//~e(),这里是不被允许的，因为编译器会对两个析构函数进行特殊处理，都变成destructor()这会他们构成隐藏，所以需要指定类域
		//e::~e(),这样我们每构造一个对象就会有两次析构，可能会存在危险
		//我们这里不用调析构，这里编译器会自动调用e中的析构，保证先析构子类再析构父类
		//子类初始化先调用父类构造再调子类构造


	}
		//:gg(l.gg)
		//,ss(l.ss)
		//,e(l)
	//调用默认构造时有三种情况
	// 内置类型 不确定初始化
	// 非内置类型 调用自己的默认构造，如果没有会报错
	// 把父辈的成员看成一个整体，调用父辈的默认构造进行初始化
	//
	//调用拷贝构造时
	//对于子类中的内置类型，会完成值拷贝，而非内置类型会调用他的拷贝构造
	//父类会调用父类的拷贝构造进行初始化

private:
	int gg;
	string ss;
};
//int main() {
//	f cccc(2, 1, "dddddddd");//子类
//	e mmm = cccc;
//	//f bbbb = mmm;只能将子类赋值给父类
//	e ccccc = cccc;
	//按理来说系统自己生成的赋值就够用，但是需要深拷贝的还需要自己实现赋值
///////////////////////////////////////////////////////////////////////////
class person {//final
public:
	static int a;//静态成员变量会被继承，但是只有一个，地址一样,共有的情况下
	 friend void print() {//注意这里说明print是person的友元函数，但是不会被继承下来，不是student的友元函数
		cout << "dddd" << endl;
	}
private:
	//person()要想person不被继承，第一种方法是将person的构造函数写到private成员中，第二种是加一个final关键字
	int name;
};
class student :public person {
public:
private:
};
int main() {
	person vvvv;
	static int a = 22;
	cout << person::a << endl;
	cout << student::a << endl;

}
