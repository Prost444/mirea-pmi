// 15 РІР°СЂРёР°РЅС‚ 

#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <map>
#include<string>

#define M_PI 3.14159265358979323846

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


double base_function(double x)
{
	return 0;
}

double f1(double x)
{
	return 2*x+5;
}

double f2(double x)
{
	return x * x + 6 * x + 5;
}

double f3(double x)
{
	return cos(2 * x) - sin(x + M_PI / 2);
}


class DefiniteIntegral 
{
private:
	double (* expression)(double);
	double step;
	double lowerLimit;
	double upperLimit; 

public:
	map <string, double (*)(double)> function_house
	{
		{"base_function", base_function},
		{"f1", f1},
		{"f2", f2},
		{"f3", f3} 
	};

	DefiniteIntegral(double (* f)(double) = base_function, double s = 1e-6, double l = 0, double u = 0)
	{
		expression = f;
		step = s;
		lowerLimit = l;
		upperLimit = u;
		cout << "\nDefiniteIntegral constructor";
	}

	DefiniteIntegral(const DefiniteIntegral& d)
	{
		cout << "\nDefiniteIntegral copy constructor";
		expression = d.expression;
		step = d.step;
		lowerLimit = d.lowerLimit;
		upperLimit = d.upperLimit;
	}

	~DefiniteIntegral() { cout << "\nDefiniteIntegral destructor"; }

	string get_function_name()
	{
		for (pair<string, double (*)(double)> i : function_house)
			if (i.second == expression)
				return i.first;
	}

	bool operator==(DefiniteIntegral d)
	{
		return true;
	}

	// Р¤СѓРЅРєС†РёСЏ РґР»СЏ РїСЂРёР±Р»РёР¶С‘РЅРЅРѕРіРѕ РІС‹С‡РёСЃР»РµРЅРёСЏ Р·РЅР°С‡РµРЅРёСЏ РёРЅС‚РµРіСЂР°Р»Р° РјРµС‚РѕРґРѕРј С‚СЂР°РїРµС†РёР№
	double approximateValue() 
	{
		double result = 0;
		for (double i = lowerLimit; i <= upperLimit; i += step) 
		{
			result += (go(i) + go(i + step)) / 2 * step;
		}
		return result;
	}

	double go(double x)
	{
		return expression(x);
	}

	friend ostream& operator<<(ostream& s, DefiniteIntegral& D);
	friend istream& operator>>(istream& s, DefiniteIntegral& D);
	friend bool solution_filter(DefiniteIntegral D);
};

bool solution_filter(DefiniteIntegral D)
{
	double filter_needs = 48;
	return fabs(D.approximateValue() - filter_needs) < D.step;
}

ostream& operator<<(ostream& s, DefiniteIntegral& D)
{
	s << "\nExpression: " << D.get_function_name() << "\nStep: " << D.step << "\nLower Limit: " << D.lowerLimit << "\nUpper Limit: " << D.upperLimit << "\nIntegral: " << D.approximateValue();
}

istream& operator>>(istream& s, DefiniteIntegral& D)
{
	string function_name;
	s >> function_name;
	D.expression = D.function_house[function_name];
	s >> D.step >> D.lowerLimit >> D.upperLimit;
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
	//РїРµСЂРµРјРµСЃС‚РёС‚СЊ РІ protected

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

	virtual Element<T>* pop() = 0;
	virtual Element<T>* push(T value) = 0;

	Element<T>* operator[](int index)
	{
		Element<T>* current = head;
		for (int i = 0; current != NULL && i < index; i++)
		{
			current = current->getNext();
		}
		return current;
	}

	virtual bool isEmpty() { return (LinkedList<T>::count == 0); }

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
};

// Р’С‹СЂР°РІРЅРёРІР°РЅРёРµ РІС‹РІРѕРґР° РїРѕ Р»РµРІРѕРјСѓ РєСЂР°СЋ РІ РІРµСЂС…РЅРµРј СЂРµРіРёСЃС‚СЂРµ
ostream& manipulator(ostream& s)
{
	return s << left << uppercase;
}

template<class T1>
ostream& operator<<(ostream& s, LinkedList<T1>& el)
{
	Element<T1>* current;
	for (current = el.head; current != NULL; current = current->getNext())
		s << manipulator << *current << "; ";
	return s;
}


template<class T>
class SingleLinkedList : public LinkedList<T>
{
public:
	SingleLinkedList<T>() : LinkedList<T>() {}
	virtual ~SingleLinkedList() { cout << "\nSingleLinkedList class destructor"; }

	virtual Element<T>* pop()
	{
		Element<T>* res = LinkedList<T>::tail;
		if (LinkedList<T>::tail == NULL) return NULL;
		if (LinkedList<T>::head == LinkedList<T>::tail)
		{
			LinkedList<T>::count = 0;
			res = LinkedList<T>::tail;
			LinkedList<T>::head = LinkedList<T>::tail = NULL;
			return res;
		}

		Element<T>* current;
		for (current = LinkedList<T>::head; current->getNext() != LinkedList<T>::tail; current = current->getNext());
		current->setNext(NULL);
		LinkedList<T>::count--;
		LinkedList<T>::tail = current;
		return res;
	}

	virtual Element<T>* push(T value)
	{
		Element<T>* newElem = new Element<T>(value);
		newElem->setNext(LinkedList<T>::head);
		LinkedList<T>::head = newElem;
		if (LinkedList<T>::tail == NULL)
		{
			LinkedList<T>::tail = LinkedList<T>::head;
		}
		LinkedList<T>::count++;
		return LinkedList<T>::head;
	}

	virtual Element<T>* insert(T value, Element<T>* previous = NULL)
	{
		if (previous == NULL || previous == LinkedList<T>::head) return push(value);
		Element<T>* inserted = new Element<T>(value);
		Element<T>* next = previous->getNext();
		previous->setNext(inserted);
		inserted->setNext(next);
		LinkedList<T>::count++;
		return inserted;
	}

	virtual Element<T>* remove(Element<T>* removed = NULL)
	{
		if (removed == NULL || removed == LinkedList<T>::tail) return pop();
		if (removed == LinkedList<T>::head)
		{
			LinkedList<T>::head = LinkedList<T>::head->getNext();
			LinkedList<T>::count--;
			return removed;
		}
		for (Element<T>* current = LinkedList<T>::head; current != NULL; current = current->getNext())
		{
			if ((current->getNext()) == removed)
			{
				current->setNext(removed->getNext());
				LinkedList<T>::count--;
				return removed;
			}
		}
	}
};


template<class T>
class DoubleLinkedList : public SingleLinkedList<T>
{
public:
	using LinkedList<T>::head;
	using LinkedList<T>::tail;
	using LinkedList<T>::count;

	DoubleLinkedList<T>() : SingleLinkedList<T>() {}
	virtual ~DoubleLinkedList() { cout << "\nDoubleLinkedList class destructor"; }

	virtual Element<T>* push(T value)
	{
		Element<T>* res = SingleLinkedList<T>::push(value);
		if (head != tail)
		{
			Element<T>* prehead = res->getNext();
			prehead->setPrev(head);
		}
		return res;
	}

	virtual Element<T>* pop()
	{
		if (head == tail) return SingleLinkedList<T>::pop();
		Element<T>* res = tail;
		Element<T>* previous = tail->getPrev();
		previous->setNext(NULL);
		tail->setPrev(NULL);
		count--;
		tail = previous;
		return res;
	}

	virtual Element<T>* insert(T value, Element<T>* previous = NULL)
	{
		Element<T>* inserted = SingleLinkedList<T>::insert(value, previous);
		if (head != tail)
		{
			if (inserted->getNext() != NULL)
			{
				inserted->getNext()->setPrev(inserted);
				inserted->setPrev(previous);
			}
		}
		return inserted;
	}

	virtual Element<T>* remove(Element<T>* current = NULL)
	{
		if (current == NULL || current == LinkedList<T>::tail) return pop();
		if (current == LinkedList<T>::head) return SingleLinkedList<T>::remove(current);

		Element<T>* removed = current; 
		Element<T>* previous = current->getPrev();
		Element<T>* next = current->getNext();

		if (next != NULL)
			next->setPrev(previous); 
		if (previous != NULL)
			previous->setNext(next); 

		LinkedList<T>::count--;
		current = NULL;
		return current;
	}

	template<class T1>
	friend ostream& operator<<(ostream& s, DoubleLinkedList<T1>& el);
};

template<class T>
ostream& operator<<(ostream& s, DoubleLinkedList<T>& el)
{
	Element<T>* current;
	for (current = el.head; current != NULL; current = current->getNext())
		s << *current << "; ";
	return s;
}




int main()
{
	DoubleLinkedList<int> test;
	for (int i = 0; i < 50; i += 5)
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

    DoubleLinkedList<DefiniteIntegral> ptr;
	ptr.push(DefiniteIntegral(f1, 1e-6, -3, 3));
	ptr.push(DefiniteIntegral(f2, 1e-6, -3, 3));
	ptr.push(DefiniteIntegral(f3, 1e-6, -3, 3));

	cout <<"\nLinkedList = " << ptr;

    DoubleLinkedList<DefiniteIntegral> filterd_ptr;
	ptr.FilterRecursive(&filterd_ptr, solution_filter);
	cout << "\nFiltered DoubleLinkedList: " << filterd_ptr;

    LinkedList<int>* list2 = new SingleLinkedList<int>();
	list2->push(3);
	list2->push(4);
	cout << "\nLinckedList list2 = " << *list2;
	SingleLinkedList<int>* list3 = dynamic_cast<SingleLinkedList<int>*>(list2);
	cout << "\nSingleLinkedList from list2 = " << *list3;
	delete list2;
    return 0;
}