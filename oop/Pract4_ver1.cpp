#include <iostream>

using namespace std;

class BaseString
{
protected:
	char* p;
	int len;
	int capacity;
public:
	BaseString(char* ptr)
	{
		cout << "\nBase Constructor 1\n";

		int i = 0;
		while (ptr[i] != '\0')
			i++;
		len = i;
		capacity = (len > 127) ? len + 1 : 128;
		p = new char[capacity];
		for (i = 0; i <= len; i++)
			p[i] = ptr[i];
	}

	BaseString(const char* ptr)
	{
		cout << "\nBase Constructor 1\n";

		int i = 0;
		while (ptr[i] != '\0')
			i++;
		len = i;
		capacity = (len > 127) ? len + 1 : 128;
		p = new char[capacity];

		for (i = 0; i <= len; i++)
			p[i] = ptr[i];
	}

	BaseString(int Capacity = 128)
	{
		cout << "\nBase Constructor 0\n";
		capacity = Capacity;
		p = new char[capacity];
		len = 0;
	}

	BaseString(const BaseString& P)
	{
		cout << "\nBaseString copy constructor";
		len = P.len;
		capacity = P.capacity;
		p = new char[capacity];
		for (int i = 0; i < len; i++)
			p[i] = P.p[i];
	}

	~BaseString()
	{
		cout << "\nBase Destructor\n";
		if (p != NULL)
			delete[] p;
		len = 0;
	}

	int Length() { return len; }
	int Capacity() { return capacity; }
	//char* get() {return p;}
	char& operator[](int i) { return p[i]; }


	BaseString& operator=(BaseString& s)
	{
		cout << "\nBase Operator = \n";
		if (capacity < s.capacity)
		{
			delete[] p;
			capacity = s.capacity;
			p = new char[capacity];
		}
		len = s.len;
		for (int i = 0; i < len; i++)
			p[i] = s.p[i];
		return *this;
	}

	virtual void print()
	{
		int i = 0;
		while (p[i] != '\0')
		{
			cout << p[i];
			i++;
		}
	}

	virtual int IndexOf(char c, int start_index = 0)
	{
		if (len == 0 || start_index<0 || start_index>=len) return -1;

		//for (int i = start_index; i < len; i++)
			//if (p[i] == c) return i;
		for (char* p1 = &p[start_index]; *p1!='\0'; p1++)
			if (*p1 == c) return p1-p;
		return -1;
	}

	bool IsPalindrome()
	{
		char* p1 = p;
		char* p2 = &p[len - 1];
		while (*p1 == *p2 && p1++ < p2--);
		return (p1 >= p2);
	}
};

class String : public BaseString
{
//protected:
	//int newfield;
public:
	String(int Capacity = 128) : BaseString(Capacity) { cout << "\nString constructor int"; }
	String(char* ptr) : BaseString(ptr) { cout << "\nString constructor char*"; } //String s(ptr);
	String(const char* ptr) : BaseString(ptr) { cout << "\nString constructor const char*"; } //String s("test");
	String(const String& P) : BaseString(P) { cout << "\nString copy constructor"; }
	String& operator=(const String& P)
	{ 
		cout << "\nString operator = ";
		return *this;
	}
	~String() { cout << "\nString destructor"; }
	virtual int IndexOf(char c, int start_index = 0)
	{
		if (start_index == 0) start_index = len - 1;
		if (len == 0 || start_index < 0 || start_index >= len) return -1;
		for (char* p1 = &p[start_index]; p1>=p ; p1--)
			if (*p1 == c) return p1 - p;
		return -1;
	}
	int pairs_sogl_gl()
	{
		int ans = 0;
		String alp1("aeiuoy"), alp2("qwrtpsdfghkjlzxcvbnm");
		for (char* p1 = &p[0]; *p1!='\0'; p1++)
			if (alp2.IndexOf(*p1) != -1 && alp1.IndexOf(*(p1+1)) != -1)
				ans += 1;
		return ans;
	}
};


int main()
{
	BaseString* ptr;
	String s("acefottarraasba"); ptr = &s;
	int index = ptr->IndexOf('t');
	cout << "\nSearch: " << index<<"\n";
	s.print();
	cout << "\n" << ptr->IsPalindrome();
	cout << "\n" << s.pairs_sogl_gl();
	return 0;
}
