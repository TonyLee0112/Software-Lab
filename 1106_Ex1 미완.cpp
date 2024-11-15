#include <complex>
#include <Eigen/Dense>
#include <cmath>
#include <iostream>
#include <fstream>
using namespace std;
using namespace Eigen;

const int N = 32;

class DFT {
public:
	int N;
	void InitializeDFT();
	VectorXcf* F;
	VectorXcf* IF;

	DFT(int);
	DFT(float*, int);
	~DFT();

	void ForwardDFT();
	void InverseDFT();
	VectorXcf x, X;
};

DFT::DFT(float* data, int Nn) {
	N = Nn;
	InitializeDFT();
	for (int i = 0; i < N; i++) x[i] = complex<float>(*(data + i), 0.0);
}
DFT::DFT(int Nn) {
	N = Nn;
	InitializeDFT();
}
DFT::~DFT() { }
void DFT::InitializeDFT()
{
	x.resize(N);
	X.resize(N);
	F = new VectorXcf[N];
	IF = new VectorXcf[N];
	complex<float> j = complex<float>(0.0, 1.0);
	complex<float> W = j * complex<float>(2.0 * 3.141592 / (float)N, 0.0);
	complex<float> WW = W;
	W = exp(W);
	for (int n = 0; n < N; n++) {
		F[n].resize(N);
		IF[n].resize(N);
		F[n](0) = IF[n](0) = 1.0;
		WW = pow(W, n);
		for (int k = 1; k < N; k++) {
			F[n](k) = F[n](k - 1) / WW;
			IF[n](k) = IF[n](k - 1) * WW;
		}
		for (int k = 0; k < N; k++)
			IF[n](k) /= complex<float>((float)N, 0.0);
	}
}
void DFT::ForwardDFT() {
	for (int k = 0; k < N; k++) X(k) = x.dot(F[k]);
}
void DFT::InverseDFT() {
	for (int n = 0; n < N; n++) x(n) = X.dot(IF[n]);
}
int main() {
	DFT mydft(N);
	// square signal
	for (int n = 0; n < N; n++) {
		mydft.x[n] = complex<float>(0.0, 0.0);
		if (n < 4 || n > N - 4) mydft.x[n] = complex<float>(1.0, 0.0);
	}

	// show orthogonality
	cout << "1. Check orthogonality by inner product between sinusoid functions======";
	cout << "1) 1st and 1st: " << mydft.F[1].dot(mydft.F[1]) << endl;
	cout << "1) 5st and 1st: " << mydft.F[5].dot(mydft.F[1]) << endl;
	cout << "1) 7st and 9st: " << mydft.F[7].dot(mydft.F[9]) << endl << endl << endl;

	// print x[n] values
	cout << "2. Print x[n] values======" << endl;
	for (int n = 0; n < N; n++) cout << mydft.x[n].real() << endl;
	cout << endl << endl;

	// run forward DFT and get X[k] values
	mydft.ForwardDFT();
	// sinc function
	cout << "3. Print X(k) values======" << endl;
	for (int k = 0; k < N; k++) cout << abs(mydft.X(k)) << endl;
	cout << endl << endl;

	// run inverse DFT and recover from X(k) to x[n] values
	mydft.InverseDFT();
	//print x[n] values
	cout << "4. Print x[n] values======" << endl << endl;
	for (int n = 0; n < N; n++) cout << mydft.x(n).real() << endl;

	return 0;
}
