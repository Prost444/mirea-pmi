//Лазарев Александр КМБО-03-22. ВАРИАНТ 16
#include <iostream>

using namespace std;

class A1
{
    protected:
        int a1;
    public:
        A1() { a1 = 0; cout << "\nA1 default constructor " << a1 << "\n"; }
        A1(int V1) { a1 = V1; cout << "\nA1 constructor " << a1 << "\n"; }
        virtual void print() { cout << "\nVariable of A1 class"; }
        virtual void show() { cout << "\na1 = " << a1; }
        ~A1() { cout << "\nA1 destructor"; }

};

class A2
{
    protected:
        int a2;
    public:
        A2() { a2 = 0; cout << "\nA2 default constructor " << a2 << "\n"; }
        A2(int V1) { a2 = V1; cout << "\nA2 constructor " << a2 << "\n"; }
        virtual void print() { cout << "\nVariable of A2 class"; }
        virtual void show() { cout << "\na2 = " << a2; }
        ~A2() { cout << "\nA2 destructor"; }
};

class B1 : virtual public A1, virtual public A2
{
protected:
    int b1;
public:
    B1(int V1, int V2, int V3) : A1(V2), A2(V3) { b1 = V1; cout << "\nB1 constructor " << b1 << "\n"; }
    virtual void print() { cout << "\nVariable of B1 class"; }
    virtual void show() { cout << "\nb1 = " << b1 << ", a1 = " << a1 << ", a2 = " << a2; }
};

class B2 : virtual public A1, virtual public A2
{
protected:
    int b2;
public:
    B2(int V1, int V2, int V3) : A1(V2), A2(V3) { b2 = V1; cout << "\nB2 constructor " << b2 << "\n"; }
    virtual void print() { cout << "\nVariable of B2 class"; }
    virtual void show() { cout << "\nb2 = " << b2 << ", a1 = " << a1 << ", a2 = " << a2; }
};


class C1 : virtual public B1,  virtual public B2
{
private:
    int c1;
public:
    C1(int V1, int V2, int V3, int V4, int V5) : B1(V2, V4, V5), B2(V3, V4, V5)
    {
        c1 = V1; cout << "\nC1 constructor " << c1 << "\n";
    }
    virtual void print() { cout << "\nVariable of C1 class"; }
    virtual void show() { cout << "\nc1 = " << c1 << ", b1 = " << b1 << ", b2 = " << b2 << ", a1 = " << a1 << ", a2 = " << a2; }
};

int main()
{
    B1 b1(1, 2, 3);
    B2 b2(4, 5, 6);
    C1 c1(1,2,3,4,5);
    c1.show();
    A1* ptr = &b1;
    ptr->show();
    ptr->print();
    return 0;
}