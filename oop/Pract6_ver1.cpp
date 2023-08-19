#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <map>
#include <iomanip>

using namespace std;


template<class T>
class Element
{
protected:
	Element* next;
	Element* prev;
	T info;
public:

	Element(T data)
	{
		next = prev = NULL;
		info = data;
	}

	Element(Element* Next, Element* Prev, T data)
	{
		next = Next;
		prev = Prev;
		info = data;
	}

	T getInfo() { return info; }
	void setInfo(T value) { info = value; }
	Element<T>* getNext() { return next; }
	void setNext(Element<T>* value) { next = value; }
	Element<T>* getPrev() { return prev; }
	void setPrev(Element<T>* value) { prev = value; }

	template<class T1>
	friend ostream& operator<<(ostream& s, Element<T1>& el);

};

template<class T1>
ostream& operator<<(ostream& s, Element<T1>& el)
{
	s << el.info;
	return s;
}

template<class T>
class LinkedList
{
protected:
	Element<T>* head;
	Element<T>* tail;
	int count;
public:
	LinkedList()
	{
		head = tail = NULL;
		count = 0;
	}

	LinkedList(T* arr, int len)
	{
		for (int i = 0; i < len; i++)
            push(arr[i]);
	}

	virtual Element<T>* pop() = 0;
	virtual Element<T>* push(T value) = 0;

	Element<T>* operator[](int index)
	{
		Element<T>* current = head;

		for (int i = 0; current != NULL && i < index; i++) {
    		current = current->getNext();
		}
		
		return current;
	}


	Element<T>* find(const T& value) const
    {
        Element<T>* current = head;
        while (current != NULL)
        {
            if (current->getInfo() == value)
                return current;
            current = current->getNext();
        }
        return NULL;
    }


	virtual Element<T>* FindRecursive(T value,Element<T>* current = NULL)
	{
		if(current == NULL)
			return FindRecursive(value,head);
			
		if(current->getInfo() == value)
			return current;

		if(current->getNext() != NULL)
			 return FindRecursive(value,current->getNext());

		return NULL;
	}


	virtual void Filter(LinkedList<T>* list, bool (*filter)(T))
	{
		for (Element<T>* current = head; current != NULL; current = current->getNext())
			if(filter(current->getInfo()))
				list->push(current->getInfo());
	}

	virtual void FilterRecursive(LinkedList<T>* list, bool (*filter)(T), Element<T>* current = NULL)
	{
		if(current == NULL)
		{
			FilterRecursive(list, filter, head);
			return;
		}
		
		if(filter(current->getInfo()))
			list->push(current->getInfo());

		if(current->getNext() != NULL)
			FilterRecursive(list, filter, current->getNext());
	}


	virtual bool isEmpty() { return (LinkedList<T>::count == 0); }

	virtual void Load(ifstream& stream)
	{
		int size;
		stream >> size;

		for(int i = 0; i < size; i++)
		{
			T data;
			stream >> data;
			push(data);
		}
	}

	virtual void Save(ofstream& stream)
	{
		stream << count << '\n';
		for (Element<T>* current = head; current != NULL; current = current->getNext())
		{
			T info = current->getInfo();
			stream << info << '\n';
		}
	}

	template<class T1>
	friend ostream& operator<<(ostream& s, LinkedList<T1>& el);
	friend ostream& showListNumbers(ostream& os, LinkedList<float>& list);
	virtual ~LinkedList()
	{
		cout << "\nBase class destructor";
		Element<T>* previous;
		for (Element<T>* current = head; current != NULL;)
		{
			previous = current;
			current = current->getNext();
			delete previous;
		}

	}
};

ostream& special_manip(ostream& os)
{
    return os << setfill('0') << setprecision(2) << fixed << setw(5);
}


template<class T1>
std::ostream& operator<<(std::ostream& s, LinkedList<T1>& el)
{
	Element<T1>* current;
	// Свой манипулятор
	for (current = el.head; current != NULL; current = current->getNext())
	{
		if (typeid(T1) == typeid(float) || typeid(T1) == typeid(double))
		{
			s << special_manip << *current << "; ";
		}
		else
			s << *current << "; ";
	}
	return s;
}

template<class T>
class Stack : public LinkedList<T>
{
public:
    using LinkedList<T>::tail;
    using LinkedList<T>::head;
    using LinkedList<T>::count;

    Stack<T>() : LinkedList<T>() {}

    virtual ~Stack() { cout << "\nStack class destructor"; }

    virtual Element<T>* pop()
    {
        Element<T>* res = head;
        if (head == NULL) return NULL;
        if (head == tail)
        {
            count = 0;
            head = tail = NULL;
            return res;
        }
		head = NULL;
		head = res->getNext();
        count--;
        return res;
    }

   	virtual Element<T>* push(T value)
	{
		Element<T>* newElem = new Element<T>(value);
		newElem->setNext(head);
		head = newElem;
		if (tail == NULL) {
			tail = head;
		}
		count++;
		return head;
	}



	virtual Element<T>* insert(T value, Element<T>* previous = NULL)
	{
		if (previous == NULL) return push(value);

		Element<T>* inserted = new Element<T>(value);
		Element<T>* next = previous->getNext();

		previous->setNext(inserted);
		inserted->setNext(next);

		LinkedList<T>::count++;

		return inserted;
	}

	
	virtual Element<T>* remove(Element<T>* removed = NULL)
    {
        if (removed == NULL || removed == head) return pop();

        for (Element<T>* current = head; current != NULL; current = current->getNext())
        {
            if ((current->getNext()) == removed)
            {
                current->setNext(removed->getNext());
               	count--;
                return removed;
            }
        }
        return NULL;
    }

};


template<class T>
class DoubleLinkedStack : public Stack<T>
{
public:
	using LinkedList<T>::head;
	using LinkedList<T>::tail;
	using LinkedList<T>::count;

	DoubleLinkedStack<T>() : Stack<T>() {}
	virtual ~DoubleLinkedStack() { cout << "\nDoubleLinkedStack class destructor"; }

	virtual Element<T>* push(T value)
	{
		Element<T>* res = Stack<T>::push(value);
		if (head != tail)
		{ 
			Element<T>* prehead = res->getNext();
			prehead->setPrev(head);
		}
		return res;
	}

	virtual Element<T>* pop()
	{
		Element<T>* res = Stack<T>::pop();
		if (head != NULL) { head->setPrev(NULL); }
		return res;
	}
	

	virtual Element<T>* insert(T value, Element<T>* previous = NULL)
	{
		Element<T>* inserted= Stack<T>::insert(value, previous);
		if (head != tail)
		{
			inserted->getNext()->setPrev(inserted);
			inserted->setPrev(previous);
		}
		return inserted;
	}

    virtual Element<T>* remove(Element<T>* cur = NULL)
	{
		if (cur == NULL) 
			return NULL;
			
		if (cur == head) 
			return pop();
		Element<T>* removed = cur;
		Element<T>* next = cur->getNext();
		Element<T>* previous = cur->getPrev();
		next->setPrev(previous);
		previous->setNext(next);
		count--;
		cur = NULL;
		return previous;
	}
};




double basefunc(double x)
{
    return 0;
}

double f1(double x)
{
    return 2*x+2;
}

double f2(double x)
{
    return x*x + 6*x+5;
}

double f3(double x)
{
    return cos(2*x) - sin(x + M_PI/2);
}

class Equation
{
private:
    double (*equation)(double);
    double eps;
    double lboard;
    double rboard;
public:
    map<string, double (*)(double)> eq_house{
    {"basefunc", basefunc},
    {"f1", f1},
    {"f2", f2},
    {"f3", f3}};

    string get_eq_name()
    {
        for (pair<string, double (*)(double)> i : eq_house)
            if (i.second == equation)
                return i.first;
    }

	Equation(double (*f)(double)=basefunc, double e=1e-6, double Left=0, double Right=0)
    { 
        equation = f; 
        eps = e;
        lboard = Left;
        rboard = Right;
        cout << "\nEquation constructor"; 
    }
	Equation(const Equation& m)
    {
        equation = m.equation; 
        eps = m.eps;
        lboard = m.lboard;
        rboard = m.rboard;
        cout << "\nEquation copy constructor"; 
    }

	~Equation() { cout << "\nEquation destructor"; }

    double go(double x)
    {
        return equation(x);
    }

	bool operator==(Equation a)
	{
		return true;
	}

	double find_solution()
	{
		double a = lboard;
		double b = rboard;
		double c;

		while (fabs(b - a) > eps)
		{
			c = (a + b) / 2.0;
			double f_a = go(a);
			double f_c = go(c);

			if (f_c == 0.0)
				return c;
			if (f_a * f_c < 0)
				b = c;
			else
				a = c;
		}

		return (a + b) / 2.0;
	}

	


	friend ostream& operator<<(ostream& s, Equation& value);
    friend istream& operator>>(istream& s, Equation& value);
	friend bool solution_filter(Equation value);

};

ostream& operator<<(ostream& s, Equation& value)
{
	s << value.get_eq_name() << " " << value.eps << " " << value.lboard << " " << value.rboard;
	return s;
}

istream& operator>>(istream& s, Equation& value)
{
    string function_name;
    s >> function_name;
    value.equation = value.eq_house[function_name];
	s >> value.eps >> value.lboard >> value.rboard;
	return s;
}

bool solution_filter(Equation value)
{
		double filter_needs = -1;
		return fabs(value.find_solution()-filter_needs) < value.eps;
}



int main()
{
	DoubleLinkedStack<int> test;
	for (int i = 0; i < 50; i+=5)
	{
		test.push(i);
		cout << '\n' << test;
	}
	test.remove(test[3]);
	cout << '\n' << test;
	test.insert(8, test[2]);
	cout << '\n' << test;
	while (!test.isEmpty())
	{
		test.pop();
		cout << '\n' << test;
	}
	cout << "\nEverithing is fine :)";
	DoubleLinkedStack<Equation> ptr;
	ptr.push(Equation(f1, 1e-6, -3, 3));
	ptr.push(Equation(f2, 1e-6, -3, 3));
	ptr.push(Equation(f3, 1e-6, -3, 3));

	cout <<"\nStack = " << ptr;

	DoubleLinkedStack<Equation> filterd_ptr;
	ptr.FilterRecursive(&filterd_ptr, solution_filter);
	cout << "\nFiltered DoublelikedStack: " << filterd_ptr;
	ofstream ostream("test.txt"); //запись в файл
	if (ostream) { filterd_ptr.Save(ostream); }
	ostream.close();
	Stack<Equation> newfilterd_ptr;
	ifstream istream("test.txt");
	if (istream) { newfilterd_ptr.Load(istream); }
	istream.close();
	cout << "\nNew Stack from file: " << newfilterd_ptr;

	Stack<float> list;
    list.push(1.23456);
    list.push(0.123);
    list.push(5);
    list.push(123.45);
	cout << '\n' << list;
	
	DoubleLinkedStack<int> list0;
	list0.push(1);
	list0.push(2);
	list0.push(3);
	Element<int>* elem = list0.find(2);
	if (elem != NULL)
		cout << "\nElement found: " << elem->getInfo();
	LinkedList<int>* list2 = new Stack<int>();
	list2->push(3);
	list2->push(4);
	cout << "\nLinckedList list2 = " << *list2;
	Stack<int>* list3 = dynamic_cast<Stack<int>*>(list2);
	cout << "\nStack from list2 = " << *list3;
	delete list2;
    return 0;
}