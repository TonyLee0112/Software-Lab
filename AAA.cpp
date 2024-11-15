#include <Eigen/Dense>
#include <random>
#include <iostream>
using namespace std;
using namespace Eigen;
const float PI = 3.141592;

class QPSK {
public:
	QPSK() {
		N = 20; // samples per symbol
		Nsymbols = 4;
		Nbits = 2; // 2^Nbits = Nsymbols
		SNR = 1;
		initialize();
		setSymbols();
	}
	QPSK(int n, int ns, int nb, float snr) {
		N = n;
		Nsymbols = ns;
		Nbits = nb;
		SNR = snr;
		initialize();
		setSymbols();
	}
	int N;
	VectorXf c0, c1;
	int Nsymbols;
	int Nbits;
	int data, decoded_data;
	float* a0;
	float* a1;
	float SNR, Eb, a, No2, sigma;
	float ad0, ad1;
	VectorXf s, n, r;
	int number_of_bits, number_of_error_bits;
	void encode();
	void decode();
	default_random_engine generator;
	normal_distribution<double> myGauss;
	void noise();
	int countError();
	void initialize();
	void setSymbols();
};
void QPSK::initialize() {
	a0 = new float[Nsymbols];
	a1 = new float[Nsymbols];
	c0.resize(N);
	c1.resize(N);
	s.resize(N);
	n.resize(N);
	r.resize(N);
	float sqt = sqrt(2.0 / (double)N);
	for (int i = 0; i < N; i++) {
		c0[i] = sqt * cos(2 * PI * i / N);
		c1[i] = sqt * sin(2 * PI * i / N);
	}
	number_of_error_bits = 0;
	cout << "[Call initialize function]" << endl;
	cout << "DM initialize SNR " << SNR << endl << endl;
}
void QPSK::encode() {
	s = a0[data] * c0 + a1[data] * c1;
	cout << "[Call encode function]" << endl;
	cout << "Input data; " << data << " (a0: " << a0[data] << " a1: " << a1[data] << ")" << endl << endl;
}
void QPSK::noise() { // r = s + n
	for (int i = 0; i < N; i++) n(i) = myGauss(generator) * sigma;
	r = s + n;
	cout << "[Call noise function]" << endl;
	cout << n.transpose() << endl << endl;
}
int QPSK::countError() {
	int Xor = data ^ decoded_data; //0 or 1
	cout << "[Call countError function]" << endl;
	cout << Xor << " = " << data << "^" << decoded_data << ": " << endl << endl;
	int count, mask;
	count = 0; mask = 1;
	for (int i = 0; i < Nbits; i++, mask *= 2)
		if ((mask & Xor) != 0) count++;

	number_of_error_bits += count;
	return count;
} 
void QPSK::decode() { //return decoded data (calculate ad0 ad1)
	ad0 = r.dot(c0);
	ad1 = r.dot(c1);
	//QPSK
	if (ad0 > 0 && ad1 > 0) decoded_data = 0;
	else if (ad0 > 0 && !(ad1 > 0)) decoded_data = 1;
	else if (!(ad0 > 0) && ad1 > 0) decoded_data = 2;
	else decoded_data = 3;
	cout << "[Call decode function]" << endl;
	cout << "QPSK decoding is done \n" << endl;
}
void QPSK::setSymbols() {
	Eb = 1.;
	No2 = Eb / SNR;
	a = sqrt(Eb); // unit signal strength QPSK 2a^2 = 2Eb
	sigma = sqrt(No2);
	a0[0] = a0[1] = a;
	a0[2] = a0[3] = -a;
	a1[0] = a1[2] = a;
	a1[1] = a1[3] = -a;
	cout << "[Call setSymbols function]" << endl;
	cout << "QPSK setsymbol " << No2 << " a " << a << endl << endl;
}
int main() {
	//digital modulation
	QPSK dm(20, 4, 2, 100);
	cout << "[Check allocated values in main]" << endl;
	cout << dm.N << " " << dm.Nbits << " " << dm.SNR << " " << dm.Nsymbols << endl << endl;
	cout << endl << endl;
	//DM test
	dm.number_of_bits = 8;
	int ii = dm.number_of_bits / dm.Nbits;
	for (int i = 0; i < ii; i++) {
		cout << "[Start: " << i << "-th symbol en/decoding]===========" << endl << endl;
		dm.data = i;
		dm.encode();
		dm.noise();
		dm.decode();
		cout << "[Check observed values]" << endl;
		cout << "Observed values (" << dm.ad0 << ". " << dm.ad1 << ") " << endl << endl;
		int countError = dm.countError();
		cout << "[Check decoded_data and errors]" << endl;
		cout << "Input: " << dm.data << " -> " << "Decoded:" << dm.decoded_data << endl;
		cout << "Error count: " << countError << ", Number_of_error_bits: " << dm.number_of_error_bits << endl << endl;
		cout << "[End: " << i << "-th symbol en/decoding]==========" << endl << endl << endl << endl;
	}
	cout << "[Check BER]" << endl;
	cout << "BER = " << dm.number_of_error_bits / (float)dm.number_of_bits << endl << endl;
	return 0;
}
