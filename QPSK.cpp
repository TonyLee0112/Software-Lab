#include <Eigen/Dense>
#include <random>
#include <iostream>
using namespace std;
using namespace Eigen;
const float PI2 = 2. * 3.141592;
// 2 Bits 송수신 -> Phase 를 4개로 나눔.
// A cos(wt + 2pi/M * i) ,M=4, i= 0,1,2,3
class QPSK {
public:
	QPSK() {
		N = 20;
		Nsymbols = 4; // Symbol 의 종류 4가지 00,01,11,10
		Nbits = 2;
		SNR = 1;
		initialize();
		setSymbols();
		number_of_bits = Nsymbols * Nbits;
	}

	QPSK(int n, int nsymbols, int nbits, float snr) {
		N = n;
		Nsymbols = nsymbols;
		Nbits = nbits;
		SNR = snr;
		initialize();
		setSymbols();
		number_of_bits = Nsymbols * Nbits;
	}
	int N;
	int Nsymbols;
	int Nbits;
	int data, decoded_data;
	float* a0, * a1;
	float SNR, Eb, a, No2, sigma;
	float ad0, ad1;
	VectorXf c0, c1;
	VectorXf s, n, r; // s(t), n(t), r(t)
	int number_of_bits;
	int number_of_error_bits;
	void encode();
	void decode();
	default_random_engine generator;
	normal_distribution<double> myGauss;
	
	void noise();
	void initialize();
	void setSymbols();
	int countError();
};
void QPSK::initialize() {
	// S = a0 * vec(c0) + a1 * vec(c1)
	// (a0,a1) = (0,0), (0,1), (1,0), (1,1)
	a0 = new float[Nsymbols];
	a1 = new float[Nsymbols];

	// c0,c1 은 N차원 Vector -> 20 차원 벡터로 기저벡터 표현
	c0.resize(N); c1.resize(N);
	float sqt = sqrt(2. / (double)N);
	for (int i = 0; i < N; i++) {
		c0(i) = sqt * cos(PI2 / N * i);
		c1(i) = sqt * sin(PI2 / N * i);
	}
	number_of_error_bits = 0;

	s.resize(N); n.resize(N); r.resize(N);
	cout << "[Initialization]" << endl;
	cout << "SNR 초기화 완료 : " << SNR << endl << endl;
}
void QPSK::encode() {
	// data point 에서 signal 값 encoding
	s = a0[data] * c0 + a1[data] * c1;
	cout << "[Encoding]" << endl;
	cout << "Input data : " << data << " (a0: " << a0[data] << " a1: " << a1[data] << ")" << endl << endl;
}
void QPSK::noise() {
	cout << "[Generate Noise]" << endl;
	for (int i = 0; i < N; i++) {
		double z = myGauss(generator); // z ~ N(0,1^2)
		n(i) = z * sigma; // n ~ N(0,sigma^2)
	}
	// Signal 에 Noise 추가
	r = s + n;
	cout << n.transpose() << endl << endl;
}

int QPSK::countError() {
	cout << "Count Error" << endl;
	// data = 0,1,2,3
	int Xor = data ^ decoded_data;
	cout << "Sent ^ Decoded : " << data << "^" << decoded_data << " = " << Xor << endl << endl;
	int count = 0;
	int mask = 1;
	for (int rep = 0; rep < Nbits; rep++, mask *= 2) {
		// Xor : data ^ decoded_data 결과를 각 비트마다 모두 저장해놨음.
		// mask 가 1칸씩 이동하면서 check
		if ((mask & Xor) != 0) count++;
	}

	number_of_error_bits += count;
	return count;
}

void QPSK::decode() {
	// a_decoded = r(t) 내적 phi(t)

	//1. Received Signal Demodulation
	ad0 = r.dot(c0); ad1 = r.dot(c1);
	
	// 2. Determine the Original signal
	// Basis function c0,c1 은 송신 측, 수신 측 모두 알고 있고,
	// (a0,a1) 좌표를 결정해야 함.
	// (A,A), (-A,A), (-A,-A), (A,-A) 중에 하나 결정.
	if (ad0 > 0 && ad1 > 0) decoded_data = 0;
	else if (ad0 > 0 && ad1 <= 0) decoded_data = 1;
	else if (ad0 <= 0 && ad1 <= 0) decoded_data = 2;
	else decoded_data = 3;

	cout << "QPSK decoding completed" << endl;
}

void QPSK::setSymbols() {
	cout << "Set Symbols" << endl;
	Eb = 1.;
	No2 = Eb / SNR;
	a = sqrt(Eb); // s(t) = sqrt(Eb) * phi(t), phi(t) = sqrt(2/T) cos wt
	sigma = sqrt(No2);
	// 좌표값
	a0[0] = a; a1[0] = a;
	a0[1] = a; a1[1] = -a;
	a0[2] = -a; a1[2] = a;
	a0[3] = -a; a1[3] = -a;
	cout << "Bias (a value) : " << a << " , Variance (Noise Power) : " << No2 << endl << endl;
}

void main() {
	// Digital Modulation
	QPSK dm(20, 4, 2, 1.0);
	cout << "[Check allocated values in main]" << endl;
	cout << "N : " << dm.N << ", Nbits : " << dm.Nbits << ", SNR : " << dm.SNR << ", Nsymbols : " << dm.Nsymbols << endl << endl;
	cout << endl << endl;

	for (int d = 0; d < dm.Nsymbols; d++) {
		cout << "[Start: " << d << "-th symbol encoding / decoding]" << endl << endl;
		dm.data = d;
		dm.encode();
		dm.noise();
		dm.decode();
		cout << "[Check the observed values]" << endl;
		cout << "Observed : (" << dm.ad0 << ", " << dm.ad1 << ") " << endl << endl;
		int countError = dm.countError();
		cout << "[Check decoded_data and errors]" << endl;
		cout << "Input: " << dm.data << " -> " << " Decoded: " << dm.decoded_data << endl;
		cout << "Error count: " << countError << " , Error bit 수 : " << dm.number_of_error_bits << endl;
		cout << "[End: " << d << "-th symbol encoding/decoding]" << endl << endl << endl << endl;
	}
	cout << "Calculated BER : " << dm.number_of_error_bits / (float)dm.number_of_bits << endl << endl;
}


