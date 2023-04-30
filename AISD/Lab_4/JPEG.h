#pragma once
#include<SFML/Graphics.hpp>
#include"Matrix.h"

using namespace sf;//включаем пространство имен sf, чтобы постоянно не писать sf::
using namespace std;

Matrix Q1(8, 8);
Matrix Q2(8, 8);


string RLE(string data) {
	unsigned l = 0;
	int count = 0;
	string coding_string = "";
	char tmp = data[0];
	int i = 0, j = 0;
	while (l < data.length()) {
		j = 0;
		while (data[l] == tmp) {
			count++;
			l++;
			if (l == data.length()) {
				break;
			}
			j++;
		}
		/*string bin_count = to_string(count);//dec32_to_bin(count);
		bin_count += ' ';
		/*coding_string += bin_int_to_char(bin_count);
		coding_string += bin_count;*/
		if (count < 256) {
			coding_string += (char)count;
			coding_string += tmp;
		}
		else {
			while (count > 0) {
				if (count - 255 >= 0) {
					char t = (char)255;
					coding_string += (signed char)255;
					coding_string += tmp;
					count -= 255;
				}
				else {
					coding_string += (char)count;
					coding_string += tmp;
					count -= 255;
				}
			}
		}
		if (l == data.length()) {
			break;
		}
		if (l != data.length()) {
			tmp = data[l];
		}
		//l++;
		count = 0;
		i++;
	}
	return coding_string;
}
string inverse_RLE(string code) {
	string data = "";
	int i = 0, count = 0;
	while (i < code.length() / 2) {
		count = (int)code[2 * i];
		if (count < 0) {
			count = 256 + count;
		}
		for (int j = 0; j < count; j++) {
			data += code[2 * i + 1];
		}
		i++;
	}
	return data;
}
void get_chanels(Image tmp_im, Matrix& R, Matrix& G, Matrix& B, int w, int h) {
	int r = 0, g = 0, b = 0;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			r = tmp_im.getPixel(j, i).r;
			g = tmp_im.getPixel(j, i).g;
			b = tmp_im.getPixel(j, i).b;
			R.set_element(r, i, j);
			G.set_element(g, i, j);
			B.set_element(b, i, j);
		}
	}
}

void subsampling(Matrix& X) {
	int count_n = X.get_n();
	int count_m = X.get_m() / 4;
	int sum = 0;
	for (int i = 0; i < count_n; i++) {
		for (int j = 0; j < count_m; j++) {
			sum = 0;
			for (int k = 0; k < 4; k++) {
				sum += X.get_element(i, j * 4 + k);
			}
			sum /= 4;
			for (int k = 0; k < 4; k++) {
				X.set_element(sum, i, j * 4 + k);
			}
		}
	}
}

double get_elem_D(int i, int k) {
	if (i == 0) {
		return 1 / (sqrt(2));
	}
	else {
		return cos(((2 * k + 1) * i * 3.14) / 16);
	}
}

double get_elem_D_T(int i, int k) {
	if (k == 0) {
		return 1 / (sqrt(2));
	}
	else {
		return cos(((2 * i + 1) * k * 3.14) / 16);
	}
}

void DCT(Matrix& X) {
	int y = 0, z = 0, x = 0;
	double d = 0;
	int u = 0;
	Matrix Y(8, 8);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			y = 0;
			z = 0;
			for (int k = 0; k < 8; k++) {
				x = X.get_element(k, j);
				d = 0.5 * get_elem_D(i, k);
				u = x * d;
				y += ceil(x * d);
			}
			y = ceil(y);
			Y.set_element(y, i, j);
		}
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			z = 0;
			for (int k = 0; k < 8; k++) {
				y = Y.get_element(k, j);
				d = 0.5 * get_elem_D_T(i, k);
				z += ceil(y * d);
			}
			z = ceil(y);
			X.set_element(z, i, j);
		}
	}
}

void inverse_DCT(Matrix& X) {
	int y = 0, z = 0, x = 0;
	double d = 0;
	int u = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			z = 0;
			for (int k = 0; k < 8; k++) {
				y = X.get_element(k, j);
				d = 0.5 * get_elem_D_T(i, k);
				z += ceil(y * d);
			}
			z = ceil(y);
			X.set_element(z, i, j);
		}
	}
}
void Quant1(Matrix& X) {
	int x = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			x = ceil(X.get_element(i, j) / Q1.get_element(i, j));
			X.set_element(x, i, j);
		}
	}
}

void Quant2(Matrix& X) {
	int x = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			x = ceil(X.get_element(i, j) / Q2.get_element(i, j));
			X.set_element(x, i, j);
		}
	}
}

void Quant1_inverse(Matrix& X) {
	int x = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			x = X.get_element(i, j) * Q1.get_element(i, j);
			X.set_element(x, i, j);
		}
	}
}

void Quant2_inverse(Matrix& X) {
	int x = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			x = X.get_element(i, j) * Q2.get_element(i, j);
			X.set_element(x, i, j);
		}
	}
}

string sig_sag_scaner(Matrix& X) {
	int i = 0, j = 0;
	int n = X.get_n();
	int m = X.get_m();
	int count = 0;
	int x = 0;
	string tmp = "";
	x = X.get_element(0, 0);
	tmp += (char)x;
	count++;
	bool is_second = false;
	int min_i = 0;
	int min_j = 0;
	while (count < n * m) {
		if (j == m - 1) {
			i++;
			min_i++;
		}
		else {
			j++;
		}
		while (j >= min_j) {
			x = X.get_element(i, j);
			tmp += (char)x;
			count++;
			if (count == n * m) {
				break;
			}
			i++;
			if (i == n) {
				break;
			}
			j--;
		}
		//j++;
		if (i != n) {
			i--;
			j++;
		}
		else {
			i--;
			//j--;
		}
		if (i == n - 1) {
			j++;
			min_j++;
		}
		else {
			i++;
		}
		while (i >= min_i) {
			x = X.get_element(i, j);
			tmp += (char)x;
			count++;
			if (count == n * m) {
				break;
			}
			j++;
			if (j == m) {
				break;
			}
			i--;
		}
		//j--;
		//i++;
		if (j != m) {
			i++;
			j--;
		}
		else {
			j--;
			//j--;
		}
		if (count == n * m) {
			break;
		}
	}
	return tmp;
}

void sig_sag_scaner_inverse(Matrix& X, string code, int H, int W) {
	int i = 0, j = 0;
	int n = X.get_n();
	int m = X.get_m();
	int x = 0;
	int count = 0;
	//tmp += (char)X.get_element(0, 0);
	x = code[0];
	X.set_element(x, 0, 0);
	count++;
	bool is_second = false;
	int min_i = 0;
	int min_j = 0;
	while (count < n * m) {
		if (j == m - 1) {
			i++;
			min_i++;
		}
		else {
			j++;
		}
		while (j >= min_j) {
			x = (int)code[count];
			X.set_element(x, i, j);
			count++;
			if (count == code.length()) {
				break;
			}
			i++;
			if (i == n) {
				break;
			}
			j--;
		}
		//j++;
		if (i != n) {
			i--;
			j++;
		}
		else {
			i--;
			//j--;
		}
		if (i == n - 1) {
			j++;
			min_j++;
		}
		else {
			i++;
		}
		while (i >= min_i) {
			x = (int)code[count];
			X.set_element(x, i, j);
			count++;
			if (count == code.length()) {
				break;
			}
			j++;
			if (j == m) {
				break;
			}
			i--;
		}
		//j--;
		//i++;
		if (j != m) {
			i++;
			j--;
		}
		else {
			j--;
			//j--;
		}
		if (count == code.length()) {
			break;
		}
	}
}

string codder(Matrix X, bool is_Y) {
	int k_n = X.get_n() / 8;
	int k_m = X.get_m() / 8;
	int number = 0;
	int b = 0;
	Matrix block(8, 8);
	string code = "";
	for (int i = 0; i < k_n; i++) {
		for (int j = 0; j < k_m; j++) {
			if (i == k_n - 1 && j == k_m - 1) {
				i = i;
				j = j;
			}
			for (int k = 0; k < 8; k++) {
				if (k == 6) {
					k = 6;
				}
				for (int l = 0; l < 8; l++) {
					int g = 8 * i + k;
					int u = 8 * j + l;
					b = X.get_element(8 * i + k, 8 * j + l);
					block.set_element(b, k, l);
				}
				k = k;
			}
			DCT(block);
			if (is_Y) {
				Quant1(block);
			}
			else {
				Quant2(block);
			}
			for (int k = 0; k < 8; k++) {
				for (int l = 0; l < 8; l++) {
					b = block.get_element(k, l);
					X.set_element(b, 8 * i + k, 8 * j + l);
				}
			}
			number++;
		}
	}
	code = sig_sag_scaner(X);
	code = RLE(code);
	return code;
}

void decoderr(string code, Matrix& X, int H, int W, bool is_Y) {
	code = inverse_RLE(code);
	sig_sag_scaner_inverse(X, code, H, W);
	int k_n = X.get_n() / 8;
	int k_m = X.get_m() / 8;
	int number = 0;
	int b = 0;
	Matrix block(8, 8);
	for (int i = 0; i < k_n; i++) {
		for (int j = 0; j < k_m; j++) {
			if (i == k_n - 1 && j == k_m - 1) {
				i = i;
				j = j;
			}
			for (int k = 0; k < 8; k++) {
				if (k == 6) {
					k = 6;
				}
				for (int l = 0; l < 8; l++) {
					int g = 8 * i + k;
					int u = 8 * j + l;
					b = X.get_element(8 * i + k, 8 * j + l);
					block.set_element(b, k, l);
				}
				k = k;
			}
			if (is_Y) {
				Quant1_inverse(block);
			}
			else {
				Quant2_inverse(block);
			}
			inverse_DCT(block);
			for (int k = 0; k < 8; k++) {
				for (int l = 0; l < 8; l++) {
					b = block.get_element(k, l);
					X.set_element(b, 8 * i + k, 8 * j + l);
				}
			}
			number++;
		}
	}

}
void code_to_matrix(string code, Matrix& X, int h, int w) {
	int x = 0;
	//int j = 0;
	int count = 0;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			x = (int)code[count];
			count++;
			X.set_element(x, i, j);
			if (count >= code.length()) {
				count = count;
				return;
			}
		}
	}

}