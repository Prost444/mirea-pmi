//Лазарев Александр КМБО-03-22 
//ВАРИАНТ 16
#include <iostream>
#include <cmath>

using namespace std;

class Polynomial2
{
private:
	//коэффициенты многочлена второй степени: coef0 + x * coef1 + x^2 * coef2
	double coef0;
	double coef1;
	double coef2;
public:
	//конструктор без параметров: Polynomial P;
	Polynomial2() { cout << "\nConstructor0 is working"; coef0 = coef1 = coef2 = 0; }
	Polynomial2(double k0) { cout << "\nConstructor0 is working"; coef1 = coef2 = 0; coef0 = k0; }
	Polynomial2(double k0, double k1, double k2) {
		cout << "\nConstructor1 is working";
		coef0 = k0; coef1 = k1; coef2 = k2;
	}
	

	//деструктор
	~Polynomial2() { cout << "\nDestructor is working"; }

	//геттеры и сеттеры
	double getCoef0() { return coef0; }
	double getCoef1() { return coef1; }
	double getCoef2() { return coef2; }

	void setCoef0(double value) { coef0 = value; }
	void setCoef1(double value) { coef1 = value; }
	void setCoef2(double value) { coef2 = value; }

	//вывод
	void print() { cout << coef2 << "x^2 + " << coef1 << "x + " << coef0; }

    //перегрузка оператора сложения для двух полиномов
	Polynomial2 operator +(Polynomial2 P){
		Polynomial2 res;
		res.coef0 = coef0 + P.coef0;
		res.coef1 = coef1 + P.coef1;
		res.coef2 = coef2 + P.coef2;
		return res;
	}

    //перегрузка оператора сложения для полинома и числа
	Polynomial2 operator +(double d){
		Polynomial2 res;
		res.coef0 = coef0 + d;
		res.coef1 = coef1;
		res.coef2 = coef2;
		return res;
	}

    //Перегрузка операции умножения для многочлена на действительное число
	Polynomial2 operator*(double d) {
		Polynomial2 res;
		res.coef0 = coef0*d;
		res.coef1 = coef1*d;
		res.coef2 = coef2*d;
		return res;
	}
	

    //перегрузка оператора * для нахождения наибольшего корня полинома
	double operator*() {
		if (coef1 == 0) {
			return -coef0/coef1;
		}
		double d = coef1*coef1 - 4*coef0*coef2;
		if (d < 0) {
			cout << "No Real Solutions" << endl;
			return NAN;
		}
		return max((-coef1 - sqrt(d))/(2*coef2),
				   (-coef1 + sqrt(d))/(2*coef2));
	}

    //объявление дружестственной функции сложения числа и многочлена
    //для возможности реализации функции без геттеров
    friend Polynomial2 operator +(double d, Polynomial2 P);

    //объявление дружестственной функции умножения числа и многочлена
    //для возможности реализации функции без геттеров
    friend Polynomial2 operator *(double d, Polynomial2 P);
}; 

//перегрузка оператора + для сложения числа и многочлена
Polynomial2 operator +(double d, Polynomial2 P) { return P+d; }

//перегрузка оператора * для умножения числа и многочлена
Polynomial2 operator *(double d, Polynomial2 P) { return P*d; }


int main()
{
	Polynomial2 P1(1, 2, 3); 
	cout << "\nP1 = "; P1.print();
	Polynomial2 P2(-1, 2, 3);
	cout << "\nP2 = "; P2.print();

    int x = 5, y = -12;
	Polynomial2 P3 = P1 + P2;
    Polynomial2 P4 = P1 + P2 + x;
    Polynomial2 P5 = y + P1 + P2;
    Polynomial2 P6 = P1 + P2 * x;
    Polynomial2 P7 = y * P1 + P2;

	cout << "\nP1 + P2 = ";  P3.print();
    cout << "\nP1 + P2 + x = ";  P4.print();
	cout << "\ny + P1 + P2 = ";  P5.print();
    cout << "\nP1 + P2*5 = "; P6.print();
    cout << "\n(-12)*P1 + P2 = "; P7.print();
	return 0;
}