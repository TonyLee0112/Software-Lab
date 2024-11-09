#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
using namespace std;

const int n = 10, N = 10000;
const float p = 1. / 6.;
float factorial(float x) {
	if (x == 0. || x == 1.) return 1.;
	return x * factorial(x - 1);
}

float calc_pmf(float p, int x, int n) {
	return pow(p, x) * pow(1. - p, n - x) * factorial(n) / (factorial(x) * factorial(n - x));
}

int main() {
	float PX[n + 1], Ex = 0., Var = 0.;
	for (int x = 0; x <= n; x++) PX[x] = 0.;
	// N 번의 시행
	for (int i = 0; i < N; i++) {
		int n6 = 0;
		for (int j = 0; j < n; j++) {
			// X ~ B(n,p)
			// n 번의 독립시행
			// n회 동안 1/6 보다 작은 확률이 나온 경우 출현 횟수 +1
			if ((float)rand() / (float)RAND_MAX < p) n6++;
		}

		PX[n6] += 1.0;
		Ex += n6;
		Var += pow(n6, 2);
	}

	// Experiment Result
	for (int x = 0; x <= n; x++) PX[x] /= (float)N;
	Ex /= (float)N;
	Var /= (float)N;
	Var -= pow(Ex, 2);

	// Theoretical : PX[n+1] mean var
	cout << "  mean " << Ex << " " << n * p << endl;
	cout << "  Var  " << Var << " " << n * p * (1 - p) << "\n\n prob. \n";
	for (int x = 0; x < n + 1; x++) {
		cout << "  " << x << " " << PX[x] << "  " << calc_pmf(p, x, n) << "\n";
	}
	return 123;

}
