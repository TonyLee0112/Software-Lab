#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

int Example1() {
	// Writing 
	fstream out("test.txt"); // make an output file object
	if (!out) {
		cout << "Cannot open test.txt.\n";
		return -1;
	}

	out << "R " << 9.9 << endl;
	out << "T " << 4.9 << "\n";
	out << "M " << 4.88 << endl;

	out.close();
	return 0;
}

int Example2() {
	ofstream file("test.txt");

	if (!file) {
		cout << "fuck";
		return 666;
	}

	file << "Apple " << 3500 << endl;
	file << "Banana " << 1800 << endl;
	file << "Kiwi " << 9000 << endl;

	file.close();

	ifstream open("test.txt");
	
	if (!open) {
		cout << "Fuck";
		return 666;
	}

	char item[20]; // 상품명 저장하기. Apple -> 5 글자니까 5 char = 5 bytes
	float price;

	open >> item >> price;
	cout << item << " " << price << "\n";
	open >> item >> price;
	cout << item << " " << price << "\n";
	open >> item >> price;
	cout << item << " " << price << "\n";


	open.close();
	return 0;
}

int Example3() {
	cout << pow(2, 3);
	return 0;
}

#define PI 3.141592
int Example4() {
	ofstream file("sine_graph.txt");
	if (!file) {
		cout << "Cannot open sine_graph.txt file.\n";
		return -1;
	}
	float t, dt, sint, T, f;
	f = 220; // 1/220 time = 1 주기
	dt = 1. / 8000.;
	T = 10. / 220.;
	for (t = 0; t < T; t += dt) {
		sint = sin(2. * PI * f * t);
		file << t << " " << sint << endl;
	}
	file.close();
}
int Drill1() {
	ofstream sig("x(t).txt");
	if (!sig) {
		cout << "Cannot open x(t).txt.\n";
		return -1;
	}

	float T, dt, f0, fc, xt;
	f0 = 100; fc = 1000;
	dt = 1. / 2000.; // 1/dt >= 2fc
	T = 10. / 100.;
	for (float t = 0; t < T; t += dt) {
		xt = sin(2. * PI * f0 * t) * cos(2. * PI * fc * t);
		sig << t << " " << xt << "\n";
	}
	sig.close();
}
int Drill2() {
	ofstream sig("x(t).txt");
	if (!sig) {
		cout << "Cannot open x(t).txt.\n";
		return -1;
	}

	float T, dt, f0, fc, xt;
	f0 = 100; fc = 1000;
	dt = 1. / 8000.;
	T = 10. / 100.;
	for (float t = 0; t < T; t += dt) {
		xt = cos(2. * PI * fc * (1. + sin(2. * PI * f0 * t) / 5. * t));
		sig << t << " " << xt << "\n";
	}
	sig.close();
}
