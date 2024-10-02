#include <iostream>
using namespace std;

void showmatrix(int, int, float*);
void matrixmult(int, int, int, float*, float*, float*);

void main() {
	float A[] = {1,2,3,4,5,6};
	float B[] = { 6,5,4,3,2,1 };
	float C[9];

	showmatrix(3, 2, A);
	showmatrix(2, 3, B);
	matrixmult(3, 2, 3, A, B, C);
	showmatrix(3, 3, C);
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

void matrixmult(int N, int K, int M, float* C, float* D, float* E) {
	// C : N * K
	// D : K * M
	// E : N * M
	for (int n = 0; n < N; n++) {
		for (int m = 0; m < M; m++) {
			int sum = 0;
			for (int k = 0; k < K; k++) {
				// E_nm = sigma (for k = 1~K) (C_nk * D_km)
				sum += C[n * K + k] * D[k * M + m];
			}
			E[n * M + m] = sum;
		}
	}
}
