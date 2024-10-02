#include <iostream>
#include <fstream>
using namespace std;

void matrixadd(int, int, float*, float*, float*);
void showmatrix(int, int, float*);

void main() {
	float A[] = { 1,2,3,4 };
	float* B;
	B = new float[4];
	B[0] = 0; B[1] = 1; B[2] = 3; B[3] = 2;
	float C[4];
	matrixadd(2, 2, A, B, C);
	showmatrix(2, 2, C);


}

void matrixadd(int n, int m, float* a, float* b, float* c) {
	int N = n * m;
	for (int i = 0; i < N; i++) {
		c[i] = a[i] + b[i];
	}
}

void showmatrix(int n, int m, float* a) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cout << a[i * m + j] << "\t";
		}
		cout << endl;
	}
}
