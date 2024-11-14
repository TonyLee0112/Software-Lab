#include <Eigen/Dense>
#include <random>
#include <iostream>
using namespace std;
using namespace Eigen;
const float PI = 3.141592;

// Digital Modulation 의 종류 : PSK, QAM etc..
// QPSK : Quadrature Phase Shift Keying
// Send a vector for every 2 bits ( 2개의 단위 벡터 송신)

class QPSK {
public:
	QPSK() {
		N = 20; // [samples / symbol] 기저벡터의 sin,cos 이 20개로 되어있다.
		Nsymbols = 4; // 4 = 00,01,10,11
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
	int Nsymbols; // 심볼의 종류 00,01,10,11 -> 4
	int Nbits; // [bits / symbol]
	int data, decoded_data;
	float* a0, * a1;
	float SNR, Eb, a, No2, sigma;
	float ad0, ad1;
	VectorXf c0, c1; // x-dim float vector
	VectorXf s, n, r; // send, noise ,receive
	int number_of_bits, number_of_error_bits;
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
	cout << "[Call initialize function]" << endl;

	a0 = new float[Nsymbols]; a1 = new float[Nsymbols]; // ptr
	// resize 하는 이유 : x-dimension 은 크기가 미정되어있으니 N 차원으로 차원 부여
	c0.resize(N); c1.resize(N); // vector
	s.resize(N); n.resize(N); r.resize(N);

	for (int i = 0; i < N; i++) {
		// 이거 왜 360 도를 N(20)등분하지? -> QPSK 면 4등분해야하는 것 아닌가?
		c0[i] = sqrt(2 / N) * cos(2 * PI / N * i);
		c1[i] = sqrt(2 / N) * sin(2 * PI / N * i);
	}
	number_of_error_bits = 0;
	cout << "DM initialize SNR : " << SNR << endl << endl;
}
void QPSK::encode() {
	// Modulation
	// data = integer scalar
	// c_i : eigenvectors, a_i : 입력값 또는 가중치
	// s : float vector
	cout << "[Call encode function]" << endl;
	s =  a0[data] * c0 + a1[data] * c1;
	cout << "Input data: " << data << " (a0 : " << a0[data] << " a1: " << a1[data] << ")" << endl << endl;
}
void QPSK::noise() {
	cout << "[Call noise function]" << endl;
	for (int i = 0; i < N; i++) n(i) = myGauss(generator) * sigma;
	r = s + n; // received = sended + noise
	cout << n.transpose() << endl << endl;
}
int QPSK::countError() {
	cout << "[Call countError function]" << endl;
	int Xor = data ^ decoded_data; // Xor : 두 값이 서로 다르면 1, 서로 다른 비트 값들에 대하여 다 모아서 이진수로 더해줌.
	cout << "Sent ^ Decoded : " << data << "^" << decoded_data << " = " << Xor << endl << endl;
	int count = 0, mask = 1;
	for (int rep = 0; rep < Nbits; mask *= 2) { // 모든 자리 수 검사
		// Nbits = 2 -> [bits/symbol] : 2개의 비트에 대하여 검사
		// mask : Xor 값을 2진수로 나타냈을 때 각 자리수를 가리키는 수
		if ((mask & Xor) != 0) count++; // 0 : Error bit 발생X

	}
	number_of_error_bits += count;
	return count;
}
void QPSK::decode() {
	cout << "[Call decode function]" << endl;
	// a decoded = received vector 과 unit vector 내적
	// 1. Received Signal Demodulation
	ad0 = r.dot(c0); ad1 = r.dot(c1);

	// 2. Determine the original signal
	// (a0,a1) = (A,A),(A,-A),(-A,A),(-A,-A) 중에 하나
	if (ad0 > 0 && ad1 > 0) decoded_data = 0;
	else if (ad0 > 0 && !(ad1 > 0)) decoded_data = 1;
	else if ((ad0 > 0) && ad1 > 0) decoded_data = 2;
	else decoded_data = 3;

	cout << "QPSK decoding is done \n" << endl;
}
void QPSK::setSymbols() {
	cout << "[Call setSymbols function]" << endl;
	Eb = 1; // Energy of Bit signal = 2 * a^2 * T_b
	No2 = Eb / SNR; // SNR = 비트 신호 에너지 / (단방향 주파수 노이즈 파워 : N_0 / 2)
	a = sqrt(Eb); // 2a^2 = 2 Eb
	sigma = sqrt(No2);
	a0[0] = a; a1[0] = a;
	a0[1] = a; a1[1] = -a;
	a0[2] = -a; a1[2] = a;
	a0[3] = -a; a1[3] = -a;
	cout << "Bias (a value) : " << a << " , Variance (Noise power) :" << No2 << endl << endl;
}

int main() {
	// Digital Modulation
	QPSK dm(20, 4, 2, 100.0);
	cout << "[Check allocated values in main]" << endl;
	cout << "N : "<< dm.N << ", Nbits : " << dm.Nbits << ", SNR :  " << dm.SNR << ", Nsymbols : " << dm.Nsymbols << endl << endl;
	cout << endl << endl;
	for (int i = 0; i < dm.Nsymbols; i++) {
		cout << "[Start: " << i << "-th symbol encoding / decoding] ========================" << endl << endl;
		dm.data = i;
		dm.encode(); // Encoding 후 송신
		dm.noise(); // Channel 통과하면서 Noise 발생
		dm.decode(); // 수신 후 Decoding
		cout << "[Check the observed values]" << endl;
		cout << "Observed : (" << dm.ad0 << ", " << dm.ad1 << ") " << endl << endl;
		int countError = dm.countError();
		cout << "[Check decoded_data and errors]" << endl;
		cout << "Input: " << dm.data << " -> " << " Decoded: " << dm.decoded_data << endl;
		cout << "Error count: " << countError << " , Number_of_error_bits: " << dm.number_of_error_bits << endl;
		cout << "[End: " << i << "-th symbol encoding / decoding] ======================== " << endl << endl << endl << endl;
	}
	cout << "[Check BER]" << endl;
	cout << "BER = " << dm.number_of_error_bits / (float)dm.number_of_bits << endl << endl;
	return 1;

}
