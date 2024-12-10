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

float xt(float t) {
	float f = 440.;
	float fc = 2000.;
	return sin(PI2 * f * t) * cos(PI2 * fc * t);
}

void main() {
	// Amplitude Modulation
	float fs = 44100.0; // Sampling frequency
	float dt = 1. / fs;
	// float df = fs / (double)N; 불필요. Frequency Resolution


	// DFT object 생성 -> Basis function 생성
	DFT mydft(N);

	// Make a Input signal
	for (int n = 0; n < N; n++) {
		// Discrete time signal 을 구현하기 위한 n*dt
		mydft.x[n] = complex<double>(xt(n * dt), 0.0);
	}

	// Print x[n] values
	cout << "1. 원래 신호 x[n]" << endl;
	for (int n = 0; n < N; n++) {
		cout << mydft.x(n).real() << " ";
	}
	cout << endl << endl;

	// Forwad DFT and get X[k] values
	mydft.ForwardDFT();

	// sinc function
	cout << "2. DFT 결과 X[k] " << endl;
	for (int k = 0 ; k < N; k++) {
		cout << abs(mydft.X(k)) << " ";
	}
	cout << endl << endl;

	// Reverse DFT and Recover from X[k] to x[n] values
	mydft.InverseDFT();

	cout << "3. IDFT 로 복원된 신호 x~[n] " << endl << endl;
	for (int n = 0; n < N; n++) {
		cout << mydft.x(n).real() << " ";
	}
}
