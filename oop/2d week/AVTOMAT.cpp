#include <iostream>
#include <cstdlib>

using namespace std;

template <class T>
class TreapNode
{
public:
    int priority;
    T data;
    T SumTreeData;
    int size;
    TreapNode* left;
    TreapNode* right;
    T Add;

    TreapNode(T dt = 0, T ad = 0) : priority(rand()), data(dt), SumTreeData(dt), 
                                                    size(1), left(nullptr), right(nullptr), Add(ad) {}
    // Конструктор копирования
    TreapNode(const TreapNode& other) :
        priority(other.priority), data(other.data), SumTreeData(other.SumTreeData), size(other.size),
        left(other.left), right(other.right), Add(other.Add) {}
    
    void recalc() 
    {
        size = 1 + SizeOf(left) + SizeOf(right);
        SumTreeData = data + CostOf(left) + CostOf(right) + Add*size;
    }

    T CostOf(TreapNode<T>* treap) { return treap == nullptr ? 0 : treap->SumTreeData + treap->Add; }
    int SizeOf(TreapNode<T>* treap) { return treap == nullptr ? 0 : treap->size; }


    template<class T1> friend ostream& operator<< (ostream& stream, const TreapNode<T1>& N);

};


template<class T>
ostream& operator<< (ostream& stream, const TreapNode<T>& N)
{
	stream << N.data;
	return stream;
}

template <class T>
class Treap 
{
public:
    TreapNode<T>* root;

    void pushAdd(TreapNode<T>* root)
    {
        if (root == nullptr) return;

        root -> data += root -> Add; 
        if (root -> left != nullptr)
        { 
            root -> left -> Add += root -> Add;
            if (root -> left -> left == nullptr && root -> left -> right == nullptr)
            {
                root -> left -> data += root -> Add;
                root -> left -> Add = 0;
            }
        }
        if (root -> right != nullptr)
        { 
            root -> right -> Add += root -> Add;
            if (root -> right -> left == nullptr && root -> right -> right == nullptr)
            {
                root -> right -> data += root -> Add;
                root -> right -> Add = 0;
            }
        }

        root->Add = 0;
    }

    TreapNode<T>* merge(TreapNode<T>* left, TreapNode<T>* right) 
    {
        pushAdd(left);
        pushAdd(right);
        if (left == nullptr) return right;
        if (right == nullptr) return left;

        TreapNode<T>* answer;
        if (left->priority > right->priority) 
        {
            left->right = merge(left->right, right);
            answer = left;
        }
        else 
        {
            right->left = merge(left, right->left);
            answer = right;
        }

        answer -> recalc(); // пересчёт!
        return answer;
    }

    void split(TreapNode<T>* root, int key, TreapNode<T>*& left, TreapNode<T>*& right)
    {
        if (root == nullptr) 
        {
            left = nullptr;
            right = nullptr;
            return;
        }

        int curIndex = root->left == nullptr ? 1 : root->left->size+1;

        root -> data += root -> Add; 
        if (root -> left != nullptr) { root -> left -> Add += root -> Add; }
        if (root -> right != nullptr) { root -> right -> Add += root -> Add; }
        root->Add = 0;

        if (curIndex <= key) 
        {
            
            TreapNode<T>* newTree = nullptr;
            split(root->right, key-curIndex, newTree, right);

            left = new TreapNode<T>(*root); // Создаем новую вершину как копию root
            left->right = newTree;
            left->recalc(); // пересчёт в L!
        }
        else 
        {
            TreapNode<T>* newTree = nullptr;
            split(root->left, key, left, newTree);

            right = new TreapNode<T>(*root); // Создаем новую вершину как копию root
            right->left = newTree;
            right->recalc(); // пересчёт в R!
        }
    }

    Treap() : root(nullptr) {}

    Treap(T* arr, int len)
    {
        Treap<T> temp;
        for (int i = 0; i < len; i++)
            temp.push(arr[i]);
        root = temp.root;
    }

    void insert_at(T data, int pos)
    {
        TreapNode<T>* new_node = new TreapNode<T>(data);
        TreapNode<T>* left;
        TreapNode<T>* right;
        split(root, pos, left, right);

        root = merge(merge(left, new_node), right);
    }

    void push(T data)
    {
        int last = root!= nullptr ? root->size : 0;
        insert_at(data, last);
    }

    void remove(int pos)
    {
        TreapNode<T>* l;
        TreapNode<T>* m;
        TreapNode<T>* r;
        split(root, pos, l, r);
        split(r, 1, m, r);
        root = merge(l, r);
        delete m;
    }

    T DataSumOn(int A, int B)
    {
        TreapNode<T>* l;
        TreapNode<T>* m;
        TreapNode<T>* r;
        split(root, A, l, r);
        split(r, B-A+1, m, r);
        T sum = m -> SumTreeData;
        root = merge(merge(l, m), r); // объединяем деревья обратно
        return sum;
    }

    TreapNode<T>* operator[](int index)
    {
        TreapNode<T>* cur = root;
        while (cur != nullptr)
        {

            int sl = cur->left != nullptr ? cur->left->size : 0;
            if (sl == index) { return cur; }
            
            cur = sl > index ? cur->left : cur->right;
            if (sl < index)
                index -= sl + 1;
        }
        return nullptr;
    }


    void inorderTraversal() 
    {
        inorderTraversal(root);
    }

    void inorderTraversal(TreapNode<T>* node, ostream& stream) 
    {
        if (node == nullptr)
        {
            return;
        }

        inorderTraversal(node->left, stream);
        stream << *node << ' ';
        inorderTraversal(node->right, stream);
    }

    void IncreaseSubTree(T value, int A, int B)
    {
        TreapNode<T>* l;
        TreapNode<T>* m;
        TreapNode<T>* r;
        split(root, A, l, r);
        split(r, B-A+1, m, r);
        m->Add += value;
        root = merge(merge(l, m), r);
    }

    TreapNode<T>* search(T key)
    {
        return search(root, key);
    }

    TreapNode<T>* search(TreapNode<T>* node, T key) 
    {
        if (node == nullptr)
        {
            return nullptr;
        }
        if (node->data == key) return node;
        if (search(node->left, key)) return search(node->left, key);
        if (search(node->right, key)) return search(node->right, key);

    }

    template<class T1> friend ostream& operator<< (ostream& stream, Treap<T1>& N);

};

template<class T>
ostream& operator<< (ostream& stream, Treap<T>& N)
{
	N.inorderTraversal(N.root, stream);
	return stream;
}

int main() {
    unsigned random_value = 12;
    srand(random_value);

    Treap<int> treap;

    treap.push(10);
    treap.push(1);
    treap.push(2);
    treap.push(7);
    treap.push(9);
    treap.push(15);
    treap.push(6);
    cout << "\nInorder Traversal:\n" << treap;

    treap.insert_at(18, 4);
    cout << "\nInorder Traversal after iserting 18 at pos 4:\n" << treap;

    treap.remove(3);   
    cout << "\nInorder Traversal after removing element from pos 3:\n" << treap;

    cout << "\nSum of data on segment [3; 5] = " << treap.DataSumOn(3, 5);
    cout << "\nElement with index 4: " << *treap[4];
    treap.IncreaseSubTree(5, 1, 5);
    cout << "\nInorder Traversal after increaising all data values on segment [1, 5]:\n" << treap;

    TreapNode<int>* found = treap.search(2);
    cout << "\nData 2 found: " << (found ? "Yes" : "No") ;
    found = treap.search(23);
    cout << "\nData 23 found: " << (found ? "Yes" : "No");

    int *a = new int[10]{0, 4, 8, 10, 0, 9, 4, 8, 4, 0};
    Treap<int> newtreap(a, 10);
    cout << "\nInorder Traversal for arraytreap:\n" << newtreap;
    return 0;
}
