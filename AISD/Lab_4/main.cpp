#include<iostream>
#include<fstream>
#include"Matrix.h"
#include<math.h>
#include<chrono>
#include"Fractal.h"
#include"JPEG.h"

using namespace std;



int main()
{
	setlocale(LC_ALL, "RUS");
		
	Image image; //создаем объект Image (изображение)
	Image compres;
	image.loadFromFile("image1.jpg");//загружаем в него файл
	image.saveToFile("input.jpg");
	int H = 5472;
	int W = 3648;
	Matrix R(W, H);
	Matrix G(W, H);
	Matrix B(W, H);
	fstream f;
	f.open("quant1.txt", ios::in);
	int q = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			f >> q;
			Q1.set_element(q, i, j);
		}
	}
	f.close();
	f.open("quant2.txt", ios::in);
	q = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			f >> q;
			Q2.set_element(q, i, j);
		}
	}
	f.close();
	Q1 = Q1 * 0.3;
	Q2 = Q2 * 0.3;
	cout << "Q1: " << '\n';
	Q1.Print();
	cout << "\n\n\n";
	cout << "Q2: " << '\n';
	Q2.Print();
	auto beg = chrono::steady_clock::now();
	get_chanels(image, R, G, B, H, W);
	
	Matrix Y(W, H);
	Matrix Cb(W, H);
	Matrix Cr(W, H);

	Y = R * 0.299 + G * 0.587 + B * 0.114;
	Cb = (B - Y) * 0.564;
	Cr = (R - Y) * 0.713;
	//subsampling(Cb);
	//subsampling(Cr);
	string code_y = "", code_cb = "", code_cr = "";
	cout << "слой 1" << '\n';
	code_y = codder(Y, true);
	cout << "слой 2" << '\n';
	code_cb = codder(Cb, false);
	cout << "слой 3" << '\n';
	code_cr = codder(Cr, false);
	

	int n = max(code_y.length(), code_cb.length());
	int n1 = code_cr.length();
	n = max(n, n1);
	int n2 = sqrt(n);
	if (n2 % 2 != 0) {
		n2++;
	}
	int k_h = 0, l_h = 0, k_w = 0, l_w = 0;
	int n3 = n2 * n2;
	int i = 0;
	while (n3 < n) {
		n3 += n2;
		i++;
	}
	int w = n2;
	int h = n2 + i;

	Matrix Y_new(h, w);
	Matrix Cb_new(h, w);
	Matrix Cr_new(h, w);

	/*Matrix R_new(h, w);
	Matrix G_new(h, w);
	Matrix B_new(h, w);*/

	
	code_to_matrix(code_y, Y_new, h, w);
	code_to_matrix(code_cb, Cb_new, h, w);
	code_to_matrix(code_cr, Cr_new, h, w);

	compres.create(w, h);

	/*R_new = Y_new + Cr_new * 1.402;
	B_new = Y_new + Cb_new * 1.772;
	G_new = Y_new - Cb_new * 0.344 - Cr_new * 0.714;*/

	Color pix;
	int r = 0, g = 0, b = 0;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			r = Y_new.get_element(i, j);
			g = Cr_new.get_element(i, j);
			b = Cb_new.get_element(i, j);
			pix.r = r;
			pix.g = g;
			pix.b = b;
			pix.a = 255;
			compres.setPixel(j, i, pix);
		}
	}
	compres.saveToFile("compres.jpg");
	auto end = chrono::steady_clock::now();
	double seconds = chrono::duration_cast<chrono::seconds>(end - beg).count();
	//double minutes = seconds / (double)60;
	cout << " time_compres: " << seconds << " sec" << '\n';

	beg = chrono::steady_clock::now();
	Image decompres;

	Matrix R_d(W, H);
	Matrix G_d(W, H);
	Matrix B_d(W, H);

	Matrix Y_d(W, H);
	Matrix Cb_d(W, H);
	Matrix Cr_d(W, H);

	get_chanels(compres, Y_d, Cr_d, Cb_d, w, h);

	/*Y_d = R_d * 0.299 + G_d * 0.587 + B_d * 0.114;
	Cb_d = (B_d - Y_d) * 0.564;
	Cr_d = (R_d - Y_d) * 0.713;*/

	
	cout << "слой 1" << '\n';
	decoderr(code_y, Y_d, H, W, true);
	cout << "слой 2" << '\n';
	decoderr(code_cb, Cb_d, H, W, false);
	cout << "слой 3" << '\n';
	decoderr(code_cr, Cr_d, H, W, false);

	R_d = Y_d + Cr_d * 1.402;
	B_d = Y_d + Cb_d * 1.772;
	G_d = Y_d - Cb_d * 0.344 - Cr_d * 0.714;

	decompres.create(H, W);
	r = 0, g = 0, b = 0;
	for (int i = 0; i < W; i++) {
		for (int j = 0; j < H; j++) {
			r = R_d.get_element(i, j);
			g = G_d.get_element(i, j);
			b = B_d.get_element(i, j);
			pix.r = r;
			pix.g = g;
			pix.b = b;
			pix.a = 255;
			decompres.setPixel(j, i, pix);
		}
	}
	decompres.saveToFile("decompres.jpg");
	end = chrono::steady_clock::now();
	seconds = chrono::duration_cast<chrono::seconds>(end - beg).count();
	//double minutes = seconds / (double)60;
	cout << " time_decompres: " << seconds << " sec" << '\n';



	cout << "FRACTAL COMPRES: " << '\0';
	beg = chrono::steady_clock::now();

	Mat img = imread("image1.jpg", IMREAD_GRAYSCALE);

	double a = 1;

	cv::Rect region(0, 0, img.cols, img.rows);

	compress(img, a, region);

	string out = to_string(a) + ".jpg";

	imwrite(out, img);
	end = chrono::steady_clock::now();
	seconds = chrono::duration_cast<chrono::seconds>(end - beg).count();
	//double minutes = seconds / (double)60;
	cout << " time_Fractal: " << seconds << " sec" << '\n';

	return 0;

}