#pragma once
#include<iostream>

using namespace std;

const int N = 10000;
const int M = 10000;

class Matrix {
private:
	int** Matr;
	int n = N;
	int m = M;
public:
	Matrix(int max_n, int max_m) {
		Matr = new int* [max_n]; // размещаем массив из 10 указателей int - это наши строки
		for (int count = 0; count < max_n; ++count) {
			Matr[count] = new int[max_m]; // это наши столбцы
		}
		for (int i = 0; i < max_n; i++) {
			for (int j = 0; j < max_m; j++) {
				Matr[i][j] = 0;
			}
		}
		n = max_n;
		m = max_m;
	}
	
	Matrix(const Matrix &p) {
		/*for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				this->Matr[i][j] = p.Matr[i][j];
			}
		}*/
		this->Matr = p.Matr;
		this->n = p.n;
		this->m = p.m;
	}
	void set_element(int value, int i, int j) {
		Matr[i][j] = value;
	}
	int get_element(int i, int j) {
		return Matr[i][j];
	}
	int get_n() {
		return this->n;
	}
	int get_m() {
		return this->m;
	}
	Matrix operator-(Matrix X) {
		Matrix answer(this->n, this->m);
		if (X.n != this->n && X.m != this->m) {
			cout << "Mistake! The sizes of the matrices are not equal!" << '\n';
			answer = *this;
		}
		else {
			for (int i = 0; i < this->n; i++) {
				for (int j = 0; j < this->m; j++) {
					answer.Matr[i][j] = this->Matr[i][j] - X.Matr[i][j];
				}
			}
		}
		return answer;
	}
	Matrix operator+(Matrix X) {
		Matrix answer(this->n, this->m);
		if (X.n != this->n && X.m != this->m) {
			cout << "Mistake! The sizes of the matrices are not equal!" << '\n';
			answer = *this;
		}
		else {
			for (int i = 0; i < this->n; i++) {
				for (int j = 0; j < this->m; j++) {
					answer.Matr[i][j] = this->Matr[i][j] + X.Matr[i][j];
				}
			}
		}
		return answer;
	}
	Matrix operator*(double a) {
		Matrix answer(this->n, this->m);
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				answer.Matr[i][j] = a * this->Matr[i][j];
			}
		}
		return answer;
	}
	void Print() {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				cout << Matr[i][j] << "  ";
			}
			cout << "\n\n";
		}

	}
};