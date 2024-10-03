#include <iostream>
using namespace std;

void showmatrix(int n, int m, float* a);
void matrixmult(int N, int K, int M, float* C, float* D, float* E);
void scalarmult(int n, int m, float a, float* b, float* c);
void matrixadd(int n, int m, float* a, float* b, float* c);
void transpose(int m, int n, float* a, float* aT);
void Drill1();

void main() {
	Drill1();
}

void Drill1() {
	float C[] = { 0.5, 0.5, 0.5, 0.5,
				  0.5, 0.5, -0.5, -0.5,
				  0.5, -0.5, -0.5, 0.5,
				  0.5, -0.5, 0.5, -0.5 }; // 4X4
	float a[] = { 10,120,108,8 }; // 4X1
	float A[4]; // 4X4 * 4X1 = 4X1
	// Original a
	cout << "Original a\n";
	showmatrix(4, 1, a);
	// C * a = A
	matrixmult(4, 4, 1, C, a, A);
	cout << "\nA\n";
	showmatrix(4, 1, A);

	// Transpose C
	float* CT;
	CT = new float[16];
	transpose(4, 4, C, CT);
	cout << "\nTransposed Matrix : CT\n";
	showmatrix(4, 4, CT);

	// CT * A = a
	matrixmult(4, 4, 1, CT, A, a);
	cout << "\nGet back to a\n";
	showmatrix(4, 1, a);
	
	// find two smallest abs(A)
	int minIndex1 = 0, minIndex2 = 1;
	for (int i = 1; i < 4; i++) {
		if (abs(A[i]) < abs(A[minIndex1])) {
			minIndex2 = minIndex1;
			minIndex1 = i;
		}
		else if (abs(A[i]) < abs(A[minIndex2]) && i != minIndex1) {
			minIndex2 = i;
		}
	}

	// make them zero
	A[minIndex1] = 0;
	A[minIndex2] = 0;

	cout << "\nTransformed A\n";
	showmatrix(4, 1, A);

	// Return to a
	float* a_prime;
	a_prime = new float[4];
	matrixmult(4, 4, 1, CT, A, a_prime);
	cout << "\nReturned a\n";
	showmatrix(4, 1, a_prime);
}

void showmatrix(int n, int m, float* a) {
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
void scalarmult(int n, int m, float a, float* b, float* c) {
	int N = n * m;
	for (int i = 0; i < N; i++) {
		c[i] = a * b[i];
	}
}
void matrixadd(int n, int m, float* a, float* b, float* c) {
	int N = n * m;
	for (int i = 0; i < N; i++) {
		c[i] = a[i] + b[i];
	}
}
void transpose(int m, int n, float* a, float* aT) {
	for (int i = 0, ii = 0; i < m; i++, ii += n) {
		for (int j = 0; j < n; j++) {
			// aT[j][i] = a[i][j]
			aT[j * m + i] = a[ii + j];
		}
	}
}
