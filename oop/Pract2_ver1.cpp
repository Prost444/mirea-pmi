//Лазарев Александр КМБО-03-22. ВАРИАНТ 16

#include <iostream>
using namespace std;

class MyArrayParent
{
protected:
	//сколько памяти выделено?
	int capacity;
	//количество элементов - сколько памяти используем
	int count;
	//массив
    double* ptr;
	
public:
	//конструкторы и деструктор
	MyArrayParent(int Dimension = 100)
	{
		cout << "\nMyArrayParent constructor";
		//выделить память под массив ptr, заполнить поля
		ptr = new double[Dimension];
		capacity = Dimension;
		count = 0;
	}
	//конструктор принимает существующий массив
	MyArrayParent(double* arr, int len)
    {
        cout << "\nMyArrayParent constructor from array";
        //заполнить массив ptr, заполнить поля
        count = len;
        capacity = len;
        ptr = new double[capacity];
        for (int i = 0; i < count; i++)
            ptr[i] = arr[i];
    }
    // конструктор копий
	MyArrayParent(const MyArrayParent& P)
	{
		cout << "\nMyArrayParent copy constructor";
		count = P.count;
		capacity = P.capacity;
		ptr = new double[capacity];
		for (int i = 0; i < count; i++)
			ptr[i] = P.ptr[i];
	}

	MyArrayParent& operator=(const MyArrayParent& P)
	{
		cout << "\noperator=";
		//почистить ptr
		if (capacity < P.capacity)
		{
			delete[] ptr;
			capacity = P.capacity;
			ptr = new double[capacity];
		}
		//delete[] ptr;
		count = P.count;
		for (int i = 0; i < count; i++)
			ptr[i] = P.ptr[i];

		return *this;
	}

	//деструктор
	~MyArrayParent()
	{
		cout << "\nMyArrayParent destructor";
		//освободить память, выделенную под ptr
		delete[] ptr;
	}

	//обращение к полям
	int Capacity() { return capacity; }
	int Size() { return count; }
	double GetComponent(int index)
	{
		if (index >= 0 && index < count)
			return ptr[index];
		else
			return -1;
	}
	void SetComponent(int index, double value)
	{
		if (index >= 0 && index < count)
			ptr[index] = value;
		else
			return;
	}

	//добавление в конец нового значения
	void push(double value)
	{
		if (count < capacity)
		{
			ptr[count] = value;
			count++;
		}
		else
		{
			if (count == capacity) 
			{	
				double *res = new double[count+1];
				for (int i = 0; i < capacity;i++)
					res[i] = ptr[i];
				res[count] = value;
				count += 1;
				capacity += 1;
				delete[] ptr;
				ptr = res;
			}
		}	
	}

	//удаление элемента с конца
	void RemoveLastValue()
	{
		if (count > 0)
		{
			ptr[count - 1] = 0;
			count--;
		}
	}

	double& operator[](int index)
	{
		//перегрузка оператора []
		return ptr[index];
	}

	void print()
	{
		cout << "\nMyArrParent, size: " << count << ", values: {";
		int i = 0;
		for (i = 0; i < count; i++)
		{
			cout << ptr[i];
			if (i != count - 1)
				cout << ", ";
		}
		cout << "}";
	}

    //поиск элемента
	int IndexOf(double value, bool bFindFromStart = true)
	{
		int i = 0;
		if (bFindFromStart == true)
		{
			for (i = 0; i < count; i++)
			{
				if (ptr[i] == value)
					return i;
			}
		}
		else
		{
			for (i = count - 1; i >= 0; i--)
			{
				if (ptr[i] == value)
					return i;
			}
		}
		return -1;
	}
};

class MyArrayChild : public MyArrayParent
{
public:
	//используем конструкторы родителя.
	MyArrayChild(int Dimension = 100) : MyArrayParent(Dimension) { cout << "\nMyArrayChild constructor"; }
    MyArrayChild(const MyArrayChild& arr) : MyArrayParent(arr) { cout << "\nMyArrayChild Copy constructor"; }
    MyArrayChild(double* arr, int len) : MyArrayParent(arr, len) { cout << "\nMyArrayChild constructor from array"; }
	~MyArrayChild() { cout << "\nMyArrayChild destructor\n"; }


	//удаление элемента
	void RemoveAt(int index = -1)
	{
		if (count == 0) return;
		if (index == -1)
		{
			RemoveLastValue();
			return;
		}
		for (int i = index; i < count - 1; i++)
			ptr[i] = ptr[i + 1];

		count--;
	}

	//вставка элемента
	void InsertAt(double value, int index = -1)
	{
		if (index == -1 || index==count)
		{
			push(value);
			return;
		}
		if (index<0 || index>count) return;

		for (int i = count; i > index; i--)
			ptr[i] = ptr[i - 1];

		ptr[index] = value;
		count++;
	}

    //Получить сумму элементов, расположенных между первым и последним нулевыми элементами 
    //(если, конечно, массив содержит не менее 2-х нулей), и значение -1, если в массиве нулей меньше 2
    double SumBetweenZeros()
    {
        int first = IndexOf(0), last = IndexOf(0, false);
        double res = 0;
        if (first == last) { return -1; }
        for (int i = first; i < last; i++)
        {
            res += ptr[i];
        }
        return res;
    }

	MyArrayChild operator+(double element)
	{
		MyArrayChild res(count);
		for (int i = 0; i < count; i++)
			res.push(ptr[i]);
		res.push(element);
		return res;
	}

	MyArrayChild SubSequence(int start=0, int len=-1)
	{
		MyArrayChild res(len);
		if (start + len > count || start<0 || len <=0) { return res; }
		for (int i = start; i < start+len; i++)
			res.push(ptr[i]);
		return res;
	}
};

class MySortedArray : public MyArrayChild
{
protected:
	int BinSearch(double value)
	{
		int low = 0, high = count - 1;
		while (low <= high)
		{
			int mid = (low + high)/2;
			if (value == ptr[mid]) { return mid; }
			else 
			{
				if (value < ptr[mid]) { high = mid - 1; }
				else { low = mid + 1; }
			}
		}
		return -1;
	}

	int BinSearch_insert(double value, int left, int right)
	{
		int middle = (left + right) / 2;

		//база рекурсии
		if (ptr[middle] == value)
			return middle;
		if (count == 1)
		{
			if (ptr[0] > value) return 0;
			return 1;
		}
		if (left + 1 == right)
		{
			if (ptr[left] > value) return left;
			if (ptr[right] < value) return right+1;
			return right;
		}

		if (ptr[middle] > value) return BinSearch_insert(value, left, middle);
		if (ptr[middle] < value) return BinSearch_insert(value, middle, right);
	}
public:
	//используем конструктор родителя.
	MySortedArray(int Dimension = 100) : MyArrayChild(Dimension) { cout << "\nMySortedArray constructor"; }
	MySortedArray(const MySortedArray& arr) : MyArrayChild(arr) { cout << "\nMyArrayChild Copy constructor"; }
    MySortedArray(double* arr, int len) : MyArrayChild(arr, len) { cout << "\nMyArrayChild constructor"; }
	~MySortedArray() { cout << "\nMySortedArray destructor\n"; }

	int IndexOf(double value, bool bFindFromStart = true)
	{
		int index = BinSearch(value); 
		if (index > 0)
			while (ptr[index-1] == value)
				index--;
		return index;
	}

	void push(double value)
	{
		if (count == 0)
		{
			MyArrayParent::push(value); 
			return;
		}
		int index = BinSearch_insert(value, 0, count-1);
		InsertAt(value, index);
	}

	double SumBetweenZeros()
	{
		if (IndexOf(0)>-1 && ptr[IndexOf(0)+1]==0) { return 0; }
		else { return -1; }
	}
};



//Результаты работы
int main()
{
	double *a = new double[10]{0, 4, 8, 10, 0, 9, 4, 8, 4, 0};
	//Конструтор, копирующий информацию из существующего массива
	MyArrayParent arr1(a, 10);
	//Конструктор копий
	MyArrayParent arr2(arr1);
	//Оператор =, [], функция print() и поиск индекса элемента
	MyArrayParent arr3 = arr2;
	arr3.print();
	cout << "\n" << arr3[3] << " " << arr3.IndexOf(4) << " " << arr3.IndexOf(4, false);

	MyArrayChild arr4(new double[10]{0, 4, 8, 10, 0, 9, 4, 8, 4, 0}, 10);
	//Удаление элемента
	arr4.RemoveAt(4);
	arr4.print();
	//Вставка элемента
	arr4.InsertAt(9, 8);
	arr4.print();
	MyArrayChild arr5(new double[7]{-1, 0, 0, 9, 4, 8, 4}, 7);
	//Выделение подстроки и оператор +
	arr5.SubSequence(2, 5).print();
	(arr5+55).print();
	//Функция из варианта
	MyArrayChild arr6(new double[2]{0, 1}, 2);
	cout << "\n" << arr4.SumBetweenZeros();
	cout << "\n" << arr5.SumBetweenZeros();
	cout << "\n" << arr6.SumBetweenZeros();

	//Бинарные индекс и вставка
	MySortedArray arr7(new double[10]{0, 0, 0, 1, 2, 3, 4, 4, 6, 10}, 10);
	arr7.push(5);
	arr7.print();
	cout << "\n" << arr7.IndexOf(5);
	//Переопределенная функция из варианта
	MySortedArray arr8(new double[2]{0, 1}, 2);
	cout << "\n" << arr7.SumBetweenZeros();
	cout << "\n" << arr8.SumBetweenZeros();
	return 0;
}