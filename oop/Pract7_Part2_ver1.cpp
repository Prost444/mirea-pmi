#include <iostream>
#include <cstring>

using namespace std;


class HeapException : public exception
{
protected:
    //сообщение об ошибке
    char* str;
public:
    HeapException(const char* s)
    {
        str = new char[strlen(s) + 1];
        strcpy_s(str, strlen(s) + 1, s);
    }
    HeapException(const HeapException& e)
    {
        str = new char[strlen(e.str) + 1];
        strcpy_s(str, strlen(e.str) + 1, e.str);
    }
    ~HeapException()
    {
        delete[] str;
    }

    //функцию вывода можно будет переопределить в производных классах, когда будет ясна конкретика
    virtual void print()
    {
        cout << "HeapException: " << str << "; " << what();
    }
};

class IndexOutOfBoundsException : public HeapException
{
protected:
    int index;
public:
    IndexOutOfBoundsException(const char* s, int i) : HeapException(s) { index = i; }
    IndexOutOfBoundsException(const IndexOutOfBoundsException& e) : HeapException(e) { index = e.index; }
    virtual void print()
    {
        cout << "IndexOutOfBoundsException: " << str << "; Index: " << index << "; " << what();
    }
};

template <class T>
class Node
{
protected:
	T value;
public:
	//установить данные в узле
	T getValue() { return value; }
	void setValue(T v) { value = v; }

	//сравнение узлов
	int operator<(Node N)
	{
		return (value < N.getValue());
	}

	int operator>(Node N)
	{
		return (value > N.getValue());
	}

	//вывод содержимого одного узла
	void print()
	{
		cout << value;
	}

	template <class T1>
	friend ostream& operator<< (ostream& stream, const Node<T1>& N);
};

template<class T1>
ostream& operator<< (ostream& stream, const Node<T1>& N)
{
	stream << N.value;
	return stream;
}

template <class T>
void print(Node<T>* N)
{
	cout << N->getValue() << "\n";
}

//куча (heap)
template <class T>
class Heap
{
protected:
	//массив
	Node<T>* arr;
	//сколько элементов добавлено
	int len;
	//сколько памяти выделено
	int size;
public:

	//доступ к вспомогательным полям кучи и оператор индекса
	int getCapacity() { return size; }
	int getCount() { return len; }

	Node<T>& operator[](int index)
	{
		if (index < 0)
			throw IndexOutOfBoundsException("Index can't be less then zero", index);
		if  (index >= len)
			throw IndexOutOfBoundsException("Index can't be greater then heap length-1", index);
		return arr[index];
	}

	//конструктор
	Heap<T>(int MemorySize = 100)
	{
		arr = new Node<T>[MemorySize];
		len = 0;
		size = MemorySize;
	}

	//поменять местами элементы arr[index1], arr[index2]
	void Swap(int index1, int index2)
	{
		if (index1 <= 0 || index1 >= len)
		{
			if (index1 < 0)
				throw IndexOutOfBoundsException("Index can't be less then zero", index1);
			if  (index1 >= len)
				throw IndexOutOfBoundsException("Index can't be greater then heap length-1", index1);
		}
		if (index2 <= 0 || index2 >= len)
		{
			if (index2 < 0)
				throw IndexOutOfBoundsException("Index can't be less then zero", index2);
			if  (index2 >= len)
				throw IndexOutOfBoundsException("Index can't be greater then heap length-1", index2);
		}
		//здесь нужна защита от дурака

		Node<T> temp;
		temp = arr[index1];
		arr[index1] = arr[index2];
		arr[index2] = temp;
	}

	//скопировать данные между двумя узлами
	void Copy(Node<T>* dest, Node<T>* source)
	{
		dest->setValue(source->getValue());
	}

	//функции получения левого, правого дочернего элемента, родителя или их индексов в массиве
	Node<T>* GetLeftChild(int index)
	{
		if (index < 0 || index * 2 >= len)
		{
			if (index < 0)
				throw IndexOutOfBoundsException("Index can't be less then zero", index);
			if  (index >= len)
				throw IndexOutOfBoundsException("Index can't be greater then heap length-1", index);
			//if  (index*2 >= len)
			//	throw IndexOutOfBoundsException("LeftChildIndex is greater then heap length-1", index);
		}
		//здесь нужна защита от дурака
		return &arr[index * 2 + 1];
	}

	Node<T>* GetRightChild(int index)
	{
		if (index < 0 || index * 2 >= len)
		{
			if (index < 0)
				throw IndexOutOfBoundsException("Index can't be less then zero", index);
			if  (index >= len)
				throw IndexOutOfBoundsException("Index can't be greater then heap length-1", index);
			//if  (index*2 >= len)
			//	throw IndexOutOfBoundsException("RightChildIndex is greater then heap length-1", index);
		}
		//здесь нужна защита от дурака

		return &arr[index * 2 + 2];
	}

	Node<T>* GetParent(int index)
	{
		if (index <= 0 || index >= len)
		{
			if (index < 0)
				throw IndexOutOfBoundsException("Index can't be less then zero", index);
			if  (index >= len)
				throw IndexOutOfBoundsException("Index can't be greater then heap length-1", index);
			//if (index == 0)
			//	throw IndexOutOfBoundsException("ParentIndex is less then zero", index);
		}
		//здесь нужна защита от дурака

		if (index % 2 == 0)
			return &arr[index / 2 - 1];
		return &arr[index / 2];
	}

	int GetLeftChildIndex(int index)
	{
		if (index < 0 || index * 2 >= len)
		{
			if (index < 0)
				throw IndexOutOfBoundsException("Index can't be less then zero", index);
			if  (index >= len)
				throw IndexOutOfBoundsException("Index can't be greater then heap length-1", index);
			//if  (index*2 >= len)
			//	throw IndexOutOfBoundsException("LeftChildIndex is greater then heap length-1", index);
		}
		//здесь нужна защита от дурака
		return index * 2 + 1;
	}

	int GetRightChildIndex(int index)
	{
		if (index < 0 || index * 2 >= len)
		{
			if (index < 0)
				throw IndexOutOfBoundsException("Index can't be less then zero", index);
			if  (index >= len)
				throw IndexOutOfBoundsException("Index can't be greater then heap length-1", index);
			//if  (index*2 >= len)
			//	throw IndexOutOfBoundsException("RightChildIndex is greater then heap length-1", index);
		}
		//здесь нужна защита от дурака

		return index * 2 + 2;
	}

	int GetParentIndex(int index)
	{
		if (index <= 0 || index >= len)
		{
			if (index < 0)
				throw IndexOutOfBoundsException("Index can't be less then zero", index);
			if  (index >= len)
				throw IndexOutOfBoundsException("Index can't be greater then heap length-1", index);
			//if (index == 0)
			//	throw IndexOutOfBoundsException("ParentIndex is less then zero", index);
		}
		//здесь нужна защита от дурака

		if (index % 2 == 0)
			return index / 2 - 1;
		return index / 2;
	}

	//восстановление свойств кучи после удаления или добавления элемента
	void Heapify(int index = 0)
	{
		int left_child_index = GetLeftChildIndex(index);
		int right_child_index = GetRightChildIndex(index);

		if (len <= left_child_index) return;

		int max_index = index;
		if (arr[left_child_index] > arr[max_index])
		{
			max_index = left_child_index;
		}
		if (right_child_index < len && arr[right_child_index] > arr[max_index])
		{
			max_index = right_child_index;
		}

		if (max_index != index)
		{
			Swap(index, max_index);
			Heapify(max_index);
		}
	}

	//просеить элемент вверх
	void SiftUp(int index = -1)
	{
		if (index == -1) index = len - 1;
		if (index == 0) return;
		int parent_index = GetParentIndex(index);
		//нужно сравнить элементы и при необходимости произвести просеивание вверх
		if (arr[index] > arr[parent_index])
		{
			Swap(index, parent_index);
			SiftUp(parent_index);
		}
	}

	//удобный интерфейс для пользователя 
	void push(T v)
	{
		Node<T>* N = new Node<T>;
		N->setValue(v);
		push(N);
	}

	//добавление элемента - вставляем его в конец массива и просеиваем вверх
	void push(Node<T>* N)
	{
		//добавить элемент и включить просеивание
		if (len < size)
		{
			arr[len] = *N;
			len++;
			SiftUp();
		}
	}

	Node<T>* ExtractMax()
	{
		//исключить максимум и запустить просеивание кучи
		//Node<T>* res = new Node<T>(arr[0]);
		Node<T>* res = new Node<T>; Copy(res, &arr[0]);
		Swap(0, len - 1);
		len--;
		if (len > 1)
			Heapify();
		return res;
	}

	//перечислить элементы кучи и применить к ним функцию
	void Straight(void(*f)(Node<T>*))
	{
		int i;
		for (i = 0; i < len; i++)
		{
			f(&arr[i]);
		}
	}

	//перебор элементов, аналогичный проходам бинарного дерева
	void PreOrder(void(*f)(Node<T>*), int index = 0)
	{
		if (index >= 0 && index < len)
			f(&arr[index]);
		if (GetLeftChildIndex(index) < len)
			PreOrder(f, GetLeftChildIndex(index));
		if (GetRightChildIndex(index) < len)
			PreOrder(f, GetRightChildIndex(index));
	}

	void InOrder(void(*f)(Node<T>*), int index = 0)
	{
		if (GetLeftChildIndex(index) < len)
			PreOrder(f, GetLeftChildIndex(index));
		if (index >= 0 && index < len)
			f(&arr[index]);
		if (GetRightChildIndex(index) < len)
			PreOrder(f, GetRightChildIndex(index));
	}

	void PostOrder(void(*f)(Node<T>*), int index = 0)
	{
		if (GetLeftChildIndex(index) < len)
			PreOrder(f, GetLeftChildIndex(index));
		if (GetRightChildIndex(index) < len)
			PreOrder(f, GetRightChildIndex(index));
		if (index >= 0 && index < len)
			f(&arr[index]);
	}

	void Remove(int index)
    {
        if (index < 0 || index >= len)
            throw IndexOutOfBoundsException("Index less or more available was submitted to Remove: ", index);
        if (index == len - 1)
            len--;
        else
        { 
            Swap(index, len - 1);
            len--;
			if (*GetParent(index) < arr[index])
            	SiftUp(index);
			else if (*GetParent(index) > arr[index])
            	Heapify(index);
        }
    }

	template <class T1>
	friend ostream& operator<< (ostream& stream, const Heap<T1>& heap);

	~Heap()
	{
		cout << "\nBase Destructor\n";
		if (arr != NULL)
			delete[] arr;
		len = 0;
		arr = NULL;
	}
};

template <class T1>
ostream& operator<< (ostream& stream, const Heap<T1>& heap)
{
	for (int i = 0; i < heap.len; i++)
		stream << heap.arr[i] << "; ";
	return stream;
}


class Patient
{
private:
    string surname;
	string name;
    string date;
	string phone;
	string address;
	string card;
	string blood;
public:

	Patient(string Surname="Not Stated",
	string Name="Not Stated",
    string Date="Not Stated",
	string Phone="Not Stated",
	string Address="Not Stated",
	string Card="Not Stated",
	string Blood="Not Stated")
    { 
        surname = Surname;
		name = Name;
		date = Date;
		phone = Phone;
		address = Address;
		card = Card;
		blood = Blood; 
        cout << "\nPatient constructor"; 
    }

	Patient(const Patient& m)
    {
        surname = m.surname;
		name = m.name;
		date = m.date;
		phone = m.phone;
		address = m.address;
		card = m.card;
		blood = m.blood;
        cout << "\nPatient copy constructor"; 
    }

	bool operator>(const Patient& other) const
	{
		if (card != other.card)
			return card > other.card;
		else if (blood != other.blood)
			return blood > other.blood;
		else if (surname != other.surname)
			return surname > other.surname;
		else
			return name > other.name;
	}

	bool operator<(const Patient& other) const
	{
		if (card != other.card)
			return card < other.card;
		else if (blood != other.blood)
			return blood < other.blood;
		else if (surname != other.surname)
			return surname < other.surname;
		else
			return name < other.name;
	}

	bool operator==(const Patient& other) const
	{
		return card == other.card && blood == other.blood && surname == other.surname && name == other.name;
	}


	~Patient() { cout << "\nPatient destructor"; }

	friend ostream& operator<<(ostream& s, const Patient& value);
    friend istream& operator>>(istream& s, const Patient& value);
};


ostream& operator<<(ostream& s, const Patient& value)
{
	s << "Patient:\nSurname: " << value.surname << "\nName: " << value.name << "\nDate of Birth: " << value.date
	<< "\nPhone Number: " << value.phone << "\nResidential Address: " << value.address << "\nMedical Card Number: " << value.card
	<< "\nBlood Group: " << value.blood << "\n";
	return s;
}

istream& operator>>(istream& s, const Patient& value)
{
	s >> value.surname >> value.name >> value.date >> value.phone >> value.address >> value.card >> value.blood;
	return s;
}


int main()
{
	try{
	Heap<int> Tree;

	Tree.push(1);
	Tree.push(-1);
	Tree.push(-2);
	Tree.push(2);
	Tree.push(5);
	Tree.push(6);
	Tree.push(-3);
	Tree.push(-4);
	Tree.push(4);
	Tree.push(3);
	cout << '\n' << Tree << '\n';
	Tree.Remove(2);
	cout << '\n' << Tree << '\n';
	
	Heap<Patient> T(5);

	Patient patient1("Smith", "John", "1990-01-01", "+123456789", "New_York", "12345678", "A+");
    Patient patient2("Johnson", "Alice", "1985-05-15", "+987654321", "Los_Angeles", "87654321", "B-");
    Patient patient3("Brown", "David", "1978-11-30", "+555555555", "Chicago", "55555555", "AB+");
    Patient patient4("Davis", "Emily", "1995-09-20", "+111111111", "Houston", "11111111", "O+");
    Patient patient5("Miller", "Sophia", "1982-03-10", "+999999999", "Miami", "99999999", "A-");

	T.push(patient2);
	T.push(patient1);
	T.push(patient3);
	T.push(patient4);
	T.push(patient5);
	
	while (T.getCount())
		cout << *(T.ExtractMax()) << "; ";
	
	}
	catch (IndexOutOfBoundsException e)
	{
		cout << "\nIndexOutOfBoundsException has been caught: "; e.print();
	}
	return 0;
}
