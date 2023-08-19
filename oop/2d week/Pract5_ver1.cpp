#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>

using namespace std;

class Exception : public exception
{
protected:
	//сообщение об ошибке
	char* str;
public:
	Exception(const char* s)
	{
		str = new char[strlen(s) + 1];
		strcpy_s(str, strlen(s) + 1, s);
	}
	Exception(const Exception& e)
	{
		str = new char[strlen(e.str) + 1];
		strcpy_s(str, strlen(e.str) + 1, e.str);
	}
	~Exception()
	{
		delete[] str;
	}

	//функцию вывода можно будет переопределить в производных классах, когда будет ясна конкретика
	virtual void print()
	{
		cout << "Exception: " << str << "; " << what();
	}
};

class InvalidOperationException : public Exception
{
protected:
	int rows, cols;
public:
	InvalidOperationException(const char* s, int Rows, int Columns) : Exception(s) { rows = Rows; cols = Columns; }
	InvalidOperationException(const InvalidOperationException& e) : Exception(e) { rows = e.rows; cols = e.cols; }
	virtual void print()
	{
		cout << "InvalidOperationException: " << str << "; Rows: " << rows << ", Columns: " << cols << "; " << what();
	}
};

class NonScalarFunctionException : public InvalidOperationException
{
public:
	NonScalarFunctionException(const char* s, int Rows, int Columns) : InvalidOperationException(s, Rows, Columns) { }
	NonScalarFunctionException(const InvalidOperationException& e) : InvalidOperationException(e) { }
	virtual void print()
	{
		cout << "NonScalarFunctionException: " << str << "; Rows: " << rows << ", Columns: " << cols << "; "<< what();
	}
};

class IndexOutOfBoundsException : public Exception
{
protected:
	int row, column;
public:
	IndexOutOfBoundsException(const char* s, int Row, int Column) : Exception(s) { row = Row; column = Column; }
	IndexOutOfBoundsException(const IndexOutOfBoundsException& e) : Exception(e) { row = e.row; column = e.column; }
	virtual void print()
	{
		cout << "IndexOutOfBoundsException: " << str << "; Row: " << row << ", Column: " << column << "; " << what();
	}
};

class WrongSizeException : public Exception
{
protected:
	int rows, cols;
public:
	WrongSizeException(const char* s, int Rows, int Columns) : Exception(s) { rows = Rows; cols = Columns; }
	WrongSizeException(const WrongSizeException& e) : Exception(e) { rows = e.rows; cols = e.cols; }
	virtual void print()
	{
		cout << "WrongSizeException: " << str << "; Rows: " << rows << ", Columns: " << cols << "; " << what();
	}
};

class NonPositiveSizeException : public WrongSizeException
{
public:
	NonPositiveSizeException(const char* s, int Rows, int Columns) : WrongSizeException(s, Rows, Columns) { }
	NonPositiveSizeException(const NonPositiveSizeException& e) : WrongSizeException(e) { }
	virtual void print()
	{
		cout << "NonPositiveSizeException: " << str << "; Rows: " << rows << ", Columns: " << cols << "; " << what();
	}
};

class TooLargeSizeException : public WrongSizeException
{
public:
	TooLargeSizeException(const char* s, int Rows, int Columns) : WrongSizeException(s, Rows, Columns) { }
	TooLargeSizeException(const TooLargeSizeException& e) : WrongSizeException(e) { }
	virtual void print()
	{
		cout << "TooLargeSizeException: " << str << "; Rows: " << rows << ", Columns: " << cols << "; " << what();
	}
};

template<class T>
class BaseMatrix
{
protected:
	T** ptr;
	int height;
	int width;
public:
	BaseMatrix(int Height = 2, int Width = 2)
	{
		//конструктор
		if (Height <= 0 || Width <= 0)
			throw NonPositiveSizeException("Matrix size can't be negative or zero in constructor int, int", Height, Width);
		if (Height > 50 || Width > 50)
			throw TooLargeSizeException("Matrix size can't be more than 50 in constructor int, int", Height, Width);
		height = Height;
		width = Width;
		ptr = new T* [height];
		for (int i = 0; i < height; i++)
			ptr[i] = new T[width];
	}

	BaseMatrix(const BaseMatrix& M)
	{
		//конструктор копий
		height = M.height;
		width = M.width;
		ptr = new T* [height];
		for (int i = 0; i < height; i++)
		{
			ptr[i] = new T[width];
			for (int j = 0; j < width; j++)
				ptr[i][j] = M.ptr[i][j];
		}
	}

	virtual ~BaseMatrix()
	{
		//деструктор
		if (ptr != NULL)
		{
			for (int i = 0; i < height; i++)
				delete[] ptr[i];
			delete[] ptr;
			ptr = NULL;
		}
		cout << "\nBase Destructor";
	}

	void print()
	{
		//вывод
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
				cout << ptr[i][j] << " ";
			cout << "\n";
		}
	}

	T* operator[](int index)
	{
		if (index < 0 || index >= height)
			throw IndexOutOfBoundsException("Index out of bounds in operator[]", index, -1);
		return ptr[index];
	}

	T& operator()(int row, int column)
	{
		if (row < 0 || row >= height || column < 0 || column >= width)
			throw IndexOutOfBoundsException("Index out of bounds in operator[]", row, column);
		return ptr[row][column];
	}
};

template<class T>
class Matrix : public BaseMatrix<T>
{
public:
	Matrix<T>(int Height = 2, int Width = 2) : BaseMatrix<T>(Height, Width) { cout << "\nMatrix constructor is working!"; }
	virtual ~Matrix() {cout << "\nDerived Destructor";}
	T operator*()
	{
		if (BaseMatrix<T>::width != BaseMatrix<T>::height)
			throw InvalidOperationException("Couldn't execute operation for rectangular matrix in operator*()", BaseMatrix<T>::height, BaseMatrix<T>::width);
		T p = 1;
		for (int i = 0; i < BaseMatrix<T>::height; i++)
		{
			p *= this->ptr[i][this->width - 1 - i];
		}
		return p;
	}

	Matrix<T>(double** Ptr, int Height, int Width)
	{
		BaseMatrix<T>::height = Height;
		BaseMatrix<T>::width = Width;
		BaseMatrix<T>::ptr = new T* [BaseMatrix<T>::height];
		for (int i = 0; i < BaseMatrix<T>::height; i++)
		{
			BaseMatrix<T>::ptr[i] = new T[BaseMatrix<T>::width];
			for (int j = 0; j < BaseMatrix<T>::width; j++)
				BaseMatrix<T>::ptr[i][j] = Ptr[i][j];
		}
		
	}

	Matrix<T>& operator=(const Matrix<T>& M)
  	{
    	if (BaseMatrix<T>::height != M.height || BaseMatrix<T>::width != M.width)
    	{
      		for (int i = 0; i < BaseMatrix<T>::height; i++)
        		delete[] BaseMatrix<T>::ptr[i];
      		delete[] BaseMatrix<T>::ptr;
      		BaseMatrix<T>::height = M.height;
      		BaseMatrix<T>::width = M.width;
      		BaseMatrix<T>::ptr = new T * [BaseMatrix<T>::height];
	  		for (int i = 0; i < BaseMatrix<T>::height; i++)
				BaseMatrix<T>::ptr[i] = new T[BaseMatrix<T>::width]; 
    	}

    	for (int i = 0; i < BaseMatrix<T>::height; i++)
    	{
      		for (int j = 0; j < BaseMatrix<T>::width; j++)
        		BaseMatrix<T>::ptr[i][j] = M.ptr[i][j];
    	}
		return *this;
  	}

	void zapoln()
	{
		int i=0, j, k = 0, p=1, n = BaseMatrix<T>::height, m = BaseMatrix<T>::width;
		while (i < n*m)
 		{
    		k++;
    		for (j=k-1;j<m-k+1;j++){ BaseMatrix<T>::ptr[k-1][j] = p++; i++; }
 
    		for (j=k;j<n-k+1;j++){ BaseMatrix<T>::ptr[j][m-k] = p++; i++; } 
 
    		for (j=m-k-1;j>=k-1;j--){ BaseMatrix<T>::ptr[n-k][j] = p++; i++; }
 
    		for (j=n-k-1;j>=k;j--){ BaseMatrix<T>::ptr[j][k-1]=p++; i++; }
		}
	}

	Matrix<T> MyFunction16(T(*scfunction)(T))
	{
		Matrix<T> res(BaseMatrix<T>::height, BaseMatrix<T>::width);
		for (int i = 0; i < BaseMatrix<T>::height; i++)
    	{
      		for (int j = 0; j < BaseMatrix<T>::width; j++)
			{	
				T cur = BaseMatrix<T>::ptr[i][j];
				cur = scfunction(cur);
				if (isnan(cur) || isinf(cur))
					throw NonScalarFunctionException("Function returns a non-scalar value for this element", BaseMatrix<T>::height, BaseMatrix<T>::width);
        		res.ptr[i][j] = cur;
			}
    	}
		return res;
	}


	template<class T1>
	friend ostream& operator<<(ostream& s, Matrix<T1> M);
	template<class T1>
	friend istream& operator>>(istream& s, Matrix<T1>& M);
};

template <class T>
ostream& operator<<(ostream& s, Matrix<T> M)
{
	if (typeid(s) == typeid(ofstream))
	{
		s << M.height << " " << M.width << " ";
		for (int i = 0; i < M.height; i++)
			for (int j = 0; j < M.width; j++)
				s << M.ptr[i][j] << " ";
	}
	else
		for (int i = 0; i < M.height; i++)
		{
			for (int j = 0; j < M.width; j++)
				s << M.ptr[i][j] << " ";
			s << "\n";
		}
	return s;
}

template <class T>
istream& operator>>(istream& s, Matrix<T>& M)
{
	if (typeid(s) == typeid(ifstream))
	{
		int w, h;
		s >> h >> w;
		if ((h != M.height || w != M.width) && !s.eof())
		{
			for (int i = 0; i < M.height; i++)
        		delete[] M.ptr[i];
      		delete[] M.ptr;
			M.height = h;
			M.width = w;
			M.ptr = new T*[M.height];
			for (int i = 0; i < M.height; i++)
				M.ptr[i] = new T[M.width];
		}
	}
	for (int i = 0; i < M.height; i++)
		for (int j = 0; j < M.width; j++)
			s >> M.ptr[i][j];
	return s;
}



template<class T>
T sq1(T x)
{
	return x/0*0;
}

template<class T>
T sq2(T x)
{
	return x*x;
}

int main()
{
	try
	{
		double **ar1 = new double* [2];
        double **ar2 = new double* [2];
        for (int i = 0; i < 2; i++)
        {
            ar1[i] = new double[3] {10.0, 20.0, 10.0};
            ar2[i] = new double[3] {-10.0, -20.0, -10.0};
        }
        Matrix<double> M1(ar1, 2, 3); //конструктор, принимающий массив типа double**
        Matrix<double> M2(ar2, 2, 3);
        M1 = M2; //оператор =
		Matrix<double> M3(5, 4);
		M3.zapoln(); //заполнение по спирали
		cout << '\n' << M1 << '\n' << M3;
		Matrix<double> M4 = M3.MyFunction16(sq2); //функция из варианта (при вызове с параметром sq1 выдаст мою собственную ошибку NonScalarFunctionException)
		cout << '\n' << M4;

		Matrix<double>* arr = new Matrix<double>[4];
		arr[0] = M1;
		arr[1] = M2;
		arr[2] = M3;
		arr[3] = M4;
		ofstream fout("test.txt"); //запись в файл
		if (fout)
		{
			for (int i = 0; i < 4; i++)
				fout << arr[i] << "\n";
			fout.close();
		}
		ifstream fin("test.txt"); //чтение из файла (при чтении матрицы произвольного размера нет утечки памяти, срабатывают Base Destructor и Derived Destructor)
		if (fin)
		{
			Matrix<int> M;
			while(!fin.eof())
			{
				try
				{
					fin >> M;
					cout << "\n" << M;  //вывод в терминал, значения совпадают
				}
				catch (exception e) { cout << "\nException is caught: " << e.what(); }
			}
			fin.close();
		}


	}
	catch (IndexOutOfBoundsException e)
	{
		cout << "\nIndexOutOfBoundsException has been caught: "; e.print();
	}
	catch (InvalidOperationException e)
	{
		cout << "\nInvalidOperationException has been caught: "; e.print();
	}
	catch (NonScalarFunctionException e)
	{
		cout << "\nNonScalarFunctionException has been caught: "; e.print();
	}
	catch (WrongSizeException e)
	{
		cout << "\nWrongSizeException has been caught: "; e.print();
	}
	catch (Exception e)
	{
		cout << "\nException has been caught: "; e.print();
	}
	catch (exception e)
	{
		cout << "\nexception has been caught: "; e.what();
	}

	return 0;
}