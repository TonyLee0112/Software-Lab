#include <iostream>
using namespace std;

void showmatrix(int, int, float*);
void transpose(int, int, float*, float* aT);

void main() {
	float A[] = {1,2,3,4,5,6};
	float* B;
	B = new float[6];
	showmatrix(3, 2, A);
	transpose(3, 2, A, B);
	showmatrix(2, 3, B);
}

void showmatrix(int n, int m, float* a) {
	cout << "matrix \n";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cout << a[i * m + j] << "\t";
		}
		cout << endl;
	}
}

void transpose(int m, int n, float* a, float* aT) {
	for (int i = 0,ii = 0; i < m; i++, ii += n) {
		for (int j = 0; j < n; j++) {
			// aT[j][i] = a[i][j]
			aT[j * m + i] = a[ii + j];
		}
	}
}
