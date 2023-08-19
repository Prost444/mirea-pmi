#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <map>
#include <iomanip>

using namespace std;

template<class T>
class Node
{
protected:
	//закрытые переменные Node N; N.data = 10 вызовет ошибку
	T data;

	//не можем хранить Node, но имеем право хранить указатель
	Node* left;
	Node* right;
	Node* parent;

	//переменная, необходимая для поддержания баланса дерева
	int height;

	virtual Node<T>* Min(Node<T>* current)
	{
		while (current->getLeft() != NULL)
			current = current->getLeft();
		return current;
	}

	virtual Node<T>* Max(Node<T>* current)
	{
		while (current->getRight() != NULL)
			current = current->getRight();
		return current;
	}
public:
	//доступные извне переменные и функции
	virtual void setData(T d) { data = d; }
	virtual T getData() { return data; }
	int getHeight() { return height; }

	virtual Node* getLeft() { return left; }
	virtual Node* getRight() { return right; }
	virtual Node* getParent() { return parent; }

	virtual void setLeft(Node* N) { left = N; }
	virtual void setRight(Node* N) { right = N; }
	virtual void setParent(Node* N) { parent = N; }

	//Конструктор. Устанавливаем стартовые значения для указателей
	Node<T>(T n)
	{
		data = n;
		left = right = parent = NULL;
		height = 1;
	}

	Node<T>()
	{
		left = NULL;
		right = NULL;
		parent = NULL;
		data = 0;
		height = 1;
	}

	virtual void print()
	{
		cout << "\n" << data;
	}

	virtual void setHeight(int h)
	{
		height = h;
	}

	Node<T>& operator=(Node<T>& s)
	{
		data = s.data;
		height = s.height;
		left = s.left;
		right = s.right;
		parent = s.parent;
	}

	template<class T1> friend ostream& operator<< (ostream& stream, const Node<T1>& N);

	Node* successor()
	{
		if (right != NULL)
			return Min(right);
		Node* current = parent;
		while (current != NULL && current->data < data) current = current->parent;
		return current;
	}
	Node* predecessor()
	{
		if (left != NULL)
			return Max(left);
		if (parent == NULL) return NULL;
		Node* current = this;
		while (current->parent != NULL && current->parent->left == current)
			{
				current = current->parent;
			}
		return current->parent;
	}


	Node<T>* operator++()
	{
		return successor();
	}

	// Переопределение оператора -- (префиксный декремент)
	Node<T>* operator--()
	{
		return predecessor();
	}

};

template<class T>
ostream& operator<< (ostream& stream, const Node<T>& N)
{
	stream << "\nNode data: " << N.data << ", height: " << N.height;
	return stream;
}

template<class T>
void print(Node<T>* N) { cout << "\n" << N->getData(); }

template<class T>
class Tree
{
protected:
	//корень - его достаточно для хранения всего дерева
	Node<T>* root;
public:
	//доступ к корневому элементу
	virtual Node<T>* getRoot() { return root; }

	//конструктор дерева: в момент создания дерева ни одного узла нет, корень смотрит в никуда
	Tree<T>() { root = NULL; }

	//функция добавления узла в дерево
	virtual Node<T>* push_R(Node<T>* N)
	{
		return push_R(N, root);
	}

	//рекуррентная функция добавления узла. Интерфейс аналогичен (добавляется корень поддерева, 
	//куда нужно добавлять узел), но вызывает сама себя - добавление в левое или правое поддерево
	virtual Node<T>* push_R(Node<T>* N, Node<T>* Current)
	{
		//не передан добавляемый узел
		if (N == NULL) return NULL;

		if (Current == NULL) Current = root;

		//пустое дерево - добавляем в корень
		if (root == NULL)
		{
			root = N;
			return root;
		}

		if (Current->getData() > N->getData())
		{
			//идем влево
			if (Current->getLeft() != NULL) return push_R(N, Current->getLeft());
			else
			{
				Current->setLeft(N);
				N->setParent(Current);
			}
		}
		if (Current->getData() < N->getData())
		{
			//идем вправо
			if (Current->getRight() != NULL) return push_R(N, Current->getRight());
			else
			{
				Current->setRight(N);
				N->setParent(Current);
			}
		}
		return N;
	}

	//функция для добавления числа. Делаем новый узел с этими данными и вызываем нужную функцию добавления в дерево
	virtual Node<T>* push(T value)
	{
		Node<T>* N = new Node<T>(value);
		return push_R(N);
	}

	//функция удаления узла из дерева
	virtual void remove(Node<T>* N)
	{
		if (N == NULL) return; // Если не нашли, то выходим

		// Если удаляемый узел имеет двух потомков
		if (N->getLeft() != NULL && N->getRight() != NULL)
		{
			Node<T>* successor = N->successor(); // Находим преемника
			N->setData(successor->getData()); // Копируем данные преемника в удаляемый узел
			N = successor; // Теперь удаляемый узел - преемник
		}

		// Если удаляемый узел имеет только одного потомка или не имеет их вообще
		Node<T>* child = NULL;

		if (N->getLeft() != NULL)
			child = N->getLeft();
		else
		{
			if (N->getRight() != NULL)
				child = N->getRight();
		}

		if (child != NULL)
			child->setParent(N->getParent()); // Устанавливаем родителя для потомка

		if (N == root)
			root = child; // Если удаляем корень, то новый корень - потомок удаляемого узла
		else
		{
			if (N == N->getParent()->getLeft())
				N->getParent()->setLeft(child); // Если удаляемый узел - левый потомок, то переставляем указатель на потомка
			else
				N->getParent()->setRight(child); // Если удаляемый узел - правый потомок, то переставляем указатель на потомка
		}
		delete N; // Удаляем узел
	}

	//поиск минимума и максимума в дереве
	virtual Node<T>* Min(Node<T>* Current = NULL)
	{
		if (root == NULL)
			return NULL;

		Node<T>* current = root;

		while (current->getLeft() != NULL)
			current = current->getLeft();

		return current;
	}

	virtual Node<T>* Max(Node<T>* Current = NULL)
	{
		if (root == NULL)
			return NULL;

		Node<T>* current = root;

		while (current->getRight() != NULL)
			current = current->getRight();

		return current;
	}

	//поиск узла в дереве
	virtual Node<T>* Find(T data) const
	{
		if (root == NULL) return NULL;
		return Find_R(data, root);
	}

	//поиск узла в дереве. Второй параметр - в каком поддереве искать, первый - что искать
	virtual Node<T>* Find_R(T data, Node<T>* Current) const
	{
		//база рекурсии
		if (Current == NULL || Current->getData() == data) return Current;
		//рекурсивный вызов
		if (Current->getData() > data) return Find_R(data, Current->getLeft());
		if (Current->getData() < data) return Find_R(data, Current->getRight());
	}

	Node<T>* operator [](T data) 
	{
		Node<T>* res = Find(data);
		if (res == NULL) cout << "\nElement " << data << " not found in tree";
		return res;
	}

	//три обхода дерева
	virtual void PreOrder(Node<T>* N, void (*f)(Node<T>*))
	{
		if (N != NULL)
			f(N);
		if (N != NULL && N->getLeft() != NULL)
			PreOrder(N->getLeft(), f);
		if (N != NULL && N->getRight() != NULL)
			PreOrder(N->getRight(), f);
	}

	//InOrder-обход даст отсортированную последовательность
	virtual void InOrder(Node<T>* N, void (*f)(Node<T>*))
	{
		if (N != NULL && N->getLeft() != NULL)
			InOrder(N->getLeft(), f);
		if (N != NULL)
			f(N);
		if (N != NULL && N->getRight() != NULL)
			InOrder(N->getRight(), f);
	}

	virtual void PostOrder(Node<T>* N, void (*f)(Node<T>*))
	{
		if (N != NULL && N->getLeft() != NULL)
			PostOrder(N->getLeft(), f);
		if (N != NULL && N->getRight() != NULL)
			PostOrder(N->getRight(), f);
		if (N != NULL)
			f(N);
	}

	template<class T1>
	friend std::ostream& operator<<(std::ostream& stream,const Tree<T1>& tree);
	
};

template<class T>
ostream& operator<<(ostream& stream, Tree<T>& tree)
{
    Node<T>* node = tree.Min();
    while (node != NULL)
    {
        T res = node->getData();
        stream << res << ' ';

        node = node->successor();
    }
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

	// переопределение оператора "больше"
    bool operator>(const Patient& other) const
    {
        if (surname != other.surname)
            return surname > other.surname;
        else
            return name > other.name;
    }

    // переопределение оператора "меньше"
    bool operator<(const Patient& other) const
    {
        if (surname != other.surname)
            return surname < other.surname;
        else
            return name < other.name;
    }

    // переопределение оператора "равно"
    bool operator==(const Patient& other) const
    {
        return surname == other.surname && name == other.name;
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





template<class T>
class AVL_Tree : public Tree<T>
{
protected:
	//определение разности высот двух поддеревьев
	int bfactor(Node<T>* p)
	{
		int hl = 0;
		int hr = 0;
		if (p->getLeft() != NULL)
			hl = p->getLeft()->getHeight();
		if (p->getRight() != NULL)
			hr = p->getRight()->getHeight();
		return (hr - hl);
	}

	//при добавлении узлов в них нет информации о балансе, т.к. не ясно, куда в дереве они попадут
	//после добавления узла рассчитываем его высоту (расстояние до корня) и редактируем высоты в узлах, где это
	//значение могло поменяться
	void fixHeight(Node<T>* p)
	{
		int hl = 0;
		int hr = 0;
		if (p->getLeft() != NULL)
			hl = p->getLeft()->getHeight();
		if (p->getRight() != NULL)
			hr = p->getRight()->getHeight();
		p->setHeight((hl > hr ? hl : hr) + 1);
	}

	//краеугольные камни АВЛ-деревьев - процедуры поворотов
	Node<T>* RotateRight(Node<T>* p) // правый поворот вокруг p
	{
		Node<T>* q = p->getLeft();
		p->setLeft(q->getRight());
		q->setRight(p);

		if (p == Tree<T>::root) Tree<T>::root = q;
		else
		{
			//if(p->getData()<p->getParent()->getData())
			if (p->getParent()->getLeft() == p)
				p->getParent()->setLeft(q);
			else
				p->getParent()->setRight(q);
		}
		q->setParent(p->getParent());
		p->setParent(q);
		if (p->getLeft() != NULL) p->getLeft()->setParent(p);

		fixHeight(p);
		fixHeight(q);
		return q;
	}

	Node<T>* RotateLeft(Node<T>* q) // левый поворот вокруг q
	{
		Node<T>* p = q->getRight();
		q->setRight(p->getLeft());
		p->setLeft(q);

		if (q == Tree<T>::root) Tree<T>::root = p;
		else
		{
			if (q->getParent()->getLeft() == q)
				q->getParent()->setLeft(p);
			else
				q->getParent()->setRight(p);
		}
		p->setParent(q->getParent());
		q->setParent(p);
		if (q->getRight() != NULL) q->getRight()->setParent(q);

		fixHeight(q);
		fixHeight(p);
		return p;
	}

	//балансировка поддерева узла p - вызов нужных поворотов в зависимости от показателя баланса
	Node<T>* Balance(Node<T>* p) // балансировка узла p
	{
		fixHeight(p);
		if (bfactor(p) == 2)
		{
			if (bfactor(p->getRight()) < 0) RotateRight(p->getRight());
			/*{
				p->setRight(RotateRight(p->getRight()));
				p->getRight()->setParent(p);
			}*/
			return RotateLeft(p);
		}
		if (bfactor(p) == -2)
		{
			if (bfactor(p->getLeft()) > 0) RotateLeft(p->getLeft());
			/*{
				p->setLeft(RotateLeft(p->getLeft()));
				p->getLeft()->setParent(p);
			}*/
			return RotateRight(p);
		}

		return p; // балансировка не нужна
	}

public:
	//конструктор AVL_Tree вызывает конструктор базового класса Tree
	AVL_Tree<T>() : Tree<T>() {}

	virtual Node<T>* push_R(Node<T>* N)
	{
		return push_R(N, Tree<T>::root);
	}

	//рекуррентная функция добавления узла. Устроена аналогично, но вызывает сама себя - добавление в левое или правое поддерево
	virtual Node<T>* push_R(Node<T>* N, Node<T>* Current)
	{
		//вызываем функцию push_R из базового класса
		Node<T>* pushedNode = Tree<T>::push_R(N, Current);
		//применяем к добавленному узлу балансировку
		if (Current != NULL)
			return Balance(Current);
		return pushedNode;
	}

	//функция для добавления числа. Делаем новый узел с этими данными и вызываем нужную функцию добавления в дерево
	virtual Node<T>* push(T n)
	{
		Node<T>* N = new Node<T>;
		N->setData(n);
		return push_R(N);
	}

	//удаление узла
	virtual void remove(Node<T>* N)
	{
		Node<T>* parent = N->getParent();
		// Вызываем метод remove родительского класса для удаления узла
		Tree<T>::remove(N);
		// Балансировка дерева после удаления узла
		if (parent != NULL)
			Balance(parent);
		return;
	}
};

template <class T>
void find_and_print(Tree<T>& tree, T searchData)
{
	Node<T>* foundNode = tree.Find(searchData);
	if (foundNode == nullptr)
	{
		cout << "Node with this data " << searchData << " not found in tree.\n";
		return;
	}

	cout << "\nNodes, greater then " << searchData << ":\n";
	for (Node<T>* node = ++(*foundNode); node != nullptr; node = ++(*node))
		cout << node->getData() << "; ";

	cout << "\nFound node: " << searchData;

	cout << "\nNodes, less then " << searchData << ":\n";
	for (Node<T>* node = --(*foundNode); node != nullptr; node = --(*node))
		cout << node->getData() << "; ";
}

template <class T>
void printNodesLessThan(const Tree<T>& tree, T searchData)
{
  Node<T>* foundNode = tree.Find(searchData);
  if (foundNode == nullptr)
  {
    cout << "Узел с данными " << searchData << " не найден в дереве.\n";
    return;
  }
	cout << foundNode;
  cout << "Узлы, меньшие чем " << searchData << ":\n";
  Node<T> node = --(*foundNode);
  for (node; (node.predecessor()) != nullptr; --node)
  {
    cout << node.getData() << " ";
  }
  cout << endl;
}






int main()
{
	Tree<Patient> T;

	Patient patient1("Smith", "John", "1990-01-01", "+123456789", "New_York", "12345678", "A+");
    Patient patient2("Johnson", "Alice", "1985-05-15", "+987654321", "Los_Angeles", "87654321", "B-");
    Patient patient3("Brown", "David", "1978-11-30", "+555555555", "Chicago", "55555555", "AB+");
    Patient patient4("Davis", "Emily", "1995-09-20", "+111111111", "Houston", "11111111", "O+");
    Patient patient5("Miller", "Sophia", "1982-03-10", "+999999999", "Miami", "99999999", "A-");

    cout << '\n' << patient1;
    cout << '\n' << patient2;
    cout << '\n' << patient3;
    cout << '\n' << patient4;
    cout << '\n' << patient5;

	T.push(patient2);
	T.push(patient1);
	T.push(patient3);
	T.push(patient4);
	T.push(patient5);


	cout << '\n' << T;
	AVL_Tree<int> tree;
    tree.push(5);
    tree.push(3);
    tree.push(7);
    tree.push(2);
    tree.push(4);
    tree.push(6);
    tree.push(8);
	tree.push(9);
    tree.push(10);
    tree.push(11);

  	find_and_print(tree, 4);

	cout << '\n' << *tree[6];
	cout << '\n' << tree;
	tree.remove(tree.Find(5));
	cout << '\n' << tree;
	return 0;
}
