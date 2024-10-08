#include <iostream>
using namespace std;

const float g = 9.8;

int Example1() {
	float v;
	float T = 10;
	float errorinpercent;
	float xbar = 0;
	float dt = 0.01;

	for (float t = 0; t < T; t += dt) {
		xbar += g * (t + dt/2) * dt;
	}
	errorinpercent = fabs(490.0 - xbar); // fabs : 실수형 절댓값 반환
		cout << "dt " << dt << " x = 490 " << xbar << " error : " << errorinpercent;
		return 1;
}

int calculate_x(float dt) {
	float v;
	float T = 10;
	float errorinpercent;
	float xbar = 0;

	for (float t = 0; t < T; t += dt) {
		xbar += g * (t + dt / 2) * dt;
	}
	errorinpercent = fabs(490.0 - xbar); // fabs : 실수형 절댓값 반환
	cout << "dt " << dt << " x = 490 " << xbar << " error : " << errorinpercent << "\n";
	return 1;
}
void Example2() {
	float ddt = 0.1;
	for (int i = 0; i < 5; i++) {
		calculate_x(ddt);
		ddt *= 0.1;
	}
}

void binary(float x) {
	float rx = x;
	float n2 = 0.5;
	int iteration = 0;
	cout << x << "\t B0.";
	while (rx != 0 && iteration++ < 20) {
		if (rx >= n2) {
			cout << "1";
			rx -= n2;
		}
		else {
			cout << "0";
			n2 /= 2;
		}
	}
	cout << endl;
}
void Example3() {
	binary(0.1);
	binary(0.01);
	binary(0.125);
	binary(0.125 / 8);
}

void Example4() {
	// dt 를 2진수로 사용해서 floating point 에 문제가 없도록 해결
	float ddt = 1.0;
	for (int i = 0; i < 5; i++) {
		calculate_x(ddt);
		ddt /= 2.0;
	}
}

#define PI 3.141592
#include <cmath>
#include <fstream>
void Drill1() {
	float Analytic = 0, Numerical = 0;
	float dt = 0.1;
	ofstream func("Integration.txt");
	for (float t = 0; t < 5; t += dt) {
		func << t << " " << Analytic << " " << Numerical << endl;
		Analytic = (1 - cos(2 * PI * t)) / (2 * PI);
		Numerical += dt * sin(2 * PI * (t+dt/2));
	}
	func.close();
}

float f1(float x) {
	return sin(2 * PI * 220 * x);
}
float f2(float x) {
	// f2(x) = f1'(x) 의 실제값
	return 440. * PI * cos(2 * PI * 220 * x);
}
float f3(float x, float dx) {
	// f1'(x) 의 근사값
	float dx2 = dx / 2;
	return (f1(x + dx2) - f1(x - dx2) / dx); // Better than f1(x+dx) - f1(x) / dx
}
void Example5() {
	ofstream myout("Diff.txt");
	float dt = 0.001, t = 0.0;
	for (int i = 0; i < 50; i++) {
		myout << t << " " << f2(t) << " " << f3(t, dt) << "\n";
		t += dt;
	}
	myout.close();
}

#include <chrono>
using ns = chrono::nanoseconds;
using get_time = chrono::steady_clock;
int answer_to_everything() {
	return sin(100) * cos(20);
}
void Example6() {
	// Computation Time
	auto start = get_time::now(); // Use auto keyword to minimize typing strokes
	for (long i = 0; i < 10000; i++) answer_to_everything();
	auto end = get_time::now();
	auto diff = end - start;
	cout << "Elapsed time is :  " << chrono::duration_cast<ns>(diff).count() << " ns " << endl;

}

#include <iomanip>
#include <ctime>
void insertionSort(int n, float* array) {
	int d;
	float t;
	for (int c = 1; c < n; c++) {
		d = c;

		while (d > 0 && array[d - 1] > array[d]) {
			t = array[d];
			array[d] = array[d - 1];
			array[d - 1] = t;
			d--;
		}
	}
}

void Example7() {
	int N = 5;
	clock_t start, end, diff;
	// test 1
	cout << "test 1\n";
	float aa[] = { 12.3,22.5,-3.0, 7.4, 0.3 };
	float bb[5];

	start = clock();
	for (int i = 0; i < 30000; i++) {
		aa[0] = 12.3; aa[1] = 22.5; aa[2] = -3.0; aa[3] = 7.4; aa[4] = 0.3;
		insertionSort(N, aa);
	}
	end = clock();
	cout << "insertionSort " << end << " - " << start << " = " << (double)(end - start) / CLOCKS_PER_SEC << endl;
}

void main() {
	Drill1();
}
