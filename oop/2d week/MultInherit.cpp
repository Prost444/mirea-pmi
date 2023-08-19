// MultInherit.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>

using namespace std;

class A1
{
protected:
	int x;
public:
	A1() { x = 0; cout << "\nA1 default constructor " << x << "\n"; }
	A1(int B) { x = B; cout << "\nA1 constructor " << x << "\n"; }
	~A1() { cout << "\nA1 destructor"; }
};

class B1 : virtual public A1
{
private:
	int d;
public:
	B1(int D, int B) : A1(B)
	{
		d = D; cout << "\nB1 constructor " << d << "\n";
	}
	void Show() { cout << "\n" << d << "\t" << x; }
};

class B2 : virtual public A1
{
private:
	int d;
public:
	B2(int D, int B) : A1(B)
	{
		d = D; cout << "\nB2 constructor " << d << "\n";
	}
	void Show() { cout << "\n" << d << "\t" << x; }
};

class C1 :  public B1,  public B2
{
private:
	int d;
public:
	C1(int D, int B1, int B2) : B1(D, B1), B2(D, B2)
	{
		d = D; cout << "\nC1 constructor " << d << "\n";
	}
	void Show() { cout << "\n" << d << "\t" << x; }
};

int _tmain(int argc, _TCHAR* argv[])
{
	B1 b1(1, 2);
	B2 b2(3, 4);

	C1 c1(1,2,3);
	c1.Show();

	char c; cin>>c;
	return 0;
}

