#include <complex>
#include <Eigen/Dense>
#include <cmath>
#include <iostream>
#include <fstream>
using namespace std;
using namespace Eigen;

const int N = 32; // 32 개의 Fourier Coefficients
class DFT {
public:
	int N; // const int 있어도 선언해야됨
	void InitializeDFT();
	VectorXcf* F, * IF; // X dim float vector

	DFT(int);
	DFT(float*, int);

	// Useless Destructor

	void ForwardDFT();
	void InverseDFT();
	VectorXcf x, X;
};
DFT::DFT(float* data, int Nn) {
	N = Nn;
	InitializeDFT();
	for (int i = 0; i < N; i++) {
		// Bring element from data ~ data + N
		x[i] = complex<float>(*(data + i), 0.0);
	}
}
DFT::DFT(int Nn) {
	N = Nn;
	InitializeDFT();
}
const float PI2 = 2 * 3.141592;
void DFT::InitializeDFT() {
	x.resize(N);
	X.resize(N);
	F = new VectorXcf[N];
	IF = new VectorXcf[N];

	complex<float> j = complex<float>(0.0, 1.0);
	complex<float> W = j * complex<float>(PI2 / (float)N, 0.0);
	complex<float> WW;
	W = exp(W);

	for (int n = 0; n < N; n++) {
		// Fourier Transform 결과도 각 항마다 N개의 합을 가지고 있음.
		F[n].resize(N);
		IF[n].resize(N);

		// F[n], IF[n] 초항 설정
		F[n](0) = IF[n](0) = 1.0;

		WW = pow(W, n);
		for (int k = 1; k < N; k++) {
			// X[k] = sigma(n=0~N-1) x[n]*exp(-j 2pi/N k n)
			F[n](k) = F[n](k - 1) / WW;

			// x[n] = 1/N * sigma(k=0~N-1) X[k]*exp(j 2pi/N k n)
			IF[n](k) = IF[n](k - 1) * WW;
		}
		for (int k = 0; k < N; k++) {
			IF[n](k) /= complex<float>((float)N, 0.0);
		}
	}
}
void DFT::ForwardDFT() {
	for (int k = 0; k < N; k++) {
		// Array 는 [] 로 꺼내고, Vector 는 ()로 꺼낸다.
		X(k) = x.dot(F[k]);
	}
}
void DFT::InverseDFT() {
	for (int n = 0; n < N; n++) {
		x(n) = X.dot(IF[n]);
	}
}

void example1() {
	// DFT object 생성 -> Basis function 들만 생김
	DFT mydft(N);
	// 사각파를 DFT 해보자

	// Make a square signal
	cout << "Input signal : Square Pulse Discrete time Signal" << endl;
	for (int n = 0; n < N; n++) {
		mydft.x[n] = complex<float>(0.0, 0.0);
		if (n<4 || n>N - 4) {
			mydft.x[n] = complex<float>(1.0, 0.0);
		}
	}

	// Show Orthogonality
	cout << "1. Check Orthogonality by inner product" << endl;
	cout << "1) 1st and 1st: " << mydft.F[1].dot(mydft.F[1]) << endl; // 자기 자신과의 내적
	cout << "2) 1st and 5th: " << mydft.F[1].dot(mydft.F[5]) << endl;
	cout << "3) 7th and 9th: " << mydft.F[7].dot(mydft.F[9]) << endl << endl << endl;

	// Print x[n] values
	cout << "2. 원래 신호 x[n]" << endl;
	for (int n = N/2; n < N-1; n++) {
		cout << mydft.x(n).real() << " ";
	}
	for (int n = 0; n < N / 2 + 1; n++) {
		cout << mydft.x(n).real() << " ";
	}
	cout << endl << endl;

	// Forwad DFT and get X[k] values
	mydft.ForwardDFT();

	// sinc function
	cout << "3. DFT 결과 X[k] " << endl;
	for (int k = N/2 ; k < N-1; k++) {
		cout << abs(mydft.X(k)) << " ";
	}
	for (int k = 0; k < N / 2 + 1; k++) {
		cout << abs(mydft.X(k)) << " ";
	}
	cout << endl << endl;

	// Reverse DFT and Recover from X[k] to x[n] values
	mydft.InverseDFT();

	cout << "4. IDFT 로 복원된 신호 x~[n] " << endl << endl;
	for (int n = N/2; n < N-1; n++) {
		cout << mydft.x(n).real() << " ";
	}
	for (int n = 0; n < N / 2 + 1; n++) {
		cout << mydft.x(n).real() << " ";
	}
}

void main() {
	example1();
}
