#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

int Example1() {
	// Writing 
	fstream out("test.txt"); // make an output file object
	if (!out) {
		cout << "Cannot open test.txt.\n";
		return -1;
	}

	out << "R " << 9.9 << endl;
	out << "T " << 4.9 << "\n";
	out << "M " << 4.88 << endl;

	out.close();
	return 0;
}
int Example2() {
	// Reading
	ifstream xx("test.txt");

	if (!xx) {
		cout << "Cannot open test.txt file.\n";
		return -1;
	}

	char item[20];
	float num;

	xx >> item >> num;
	cout << item << " " << num << "\n";
	xx >> item >> num;
	cout << item << " " << num << "\n";
	xx >> item >> num;
	cout << item << " " << num << "\n";

	xx.close();
 
	return 0;
}
int Example3() {
	cout << pow(2, 3);
	return 0;
}

#define PI 3.141592
int Example4() {
	ofstream file("sine_graph.txt");
	if (!file) {
		cout << "Cannot open sine_graph.txt file.\n";
		return -1;
	}
	float t, dt, sint, T, f;
	f = 220; // 1/220 time = 1 주기
	dt = 1. / 8000.;
	T = 10. / 220.;
	for (t = 0; t < T; t += dt) {
		sint = sin(2. * PI * f * t);
		file << t << " " << sint << endl;
	}
	file.close();
}
int Drill1() {
	ofstream sig("x(t).txt");
	if (!sig) {
		cout << "Cannot open x(t).txt.\n";
		return -1;
	}

	float T, dt, f0, fc, xt;
	f0 = 100; fc = 1000;
	dt = 1. / 2000.; // 1/dt >= 2fc
	T = 10. / 100.;
	for (float t = 0; t < T; t += dt) {
		xt = sin(2. * PI * f0 * t) * cos(2. * PI * fc * t);
		sig << t << " " << xt << "\n";
	}
	sig.close();
}
int Drill2() {
	ofstream sig("x(t).txt");
	if (!sig) {
		cout << "Cannot open x(t).txt.\n";
		return -1;
	}

	float T, dt, f0, fc, xt;
	f0 = 100; fc = 1000;
	dt = 1. / 8000.;
	T = 10. / 100.;
	for (float t = 0; t < T; t += dt) {
		xt = cos(2. * PI * fc * (1. + sin(2. * PI * f0 * t) / 5. * t));
		sig << t << " " << xt << "\n";
	}
	sig.close();
}

class waveHeader {
public:
	waveHeader() {};
	int chunkID, chunkSize, Format, subChunkID, subChunk1size;
	short audioFormat, numChannels;
	int sampleRate, byteRate;
	short blockAlign, bitsPerSample;
	int subChunk2ID, subChunk2size;
};
int Exercise1() {
	ifstream xx("Beatles.wav", ios::binary | ios::in);
	if (!xx) return 999;
	waveHeader myHeader;
	xx.read((char*)&myHeader, sizeof(waveHeader));
	cout << " SampleRate = " << myHeader.sampleRate << endl;
	xx.close();
}
void Exercise2() {
	short* data, leftdata, rightdata;
	waveHeader myHeader;
	int n = myHeader.sampleRate * 5; // for 5 seconds n = 44100 * 5
	data = new short[n * myHeader.numChannels]; // short variables 의 공간을 만들어둔다.
	float dt = 1. / (float)(myHeader.sampleRate);
	float f1 = 261.63, f2 = 329.63;
	float A = 10000.0;
	for (int i = 0; i < n; i += 1) {
		float t = i * dt;
		leftdata = (short)(A * cos(2. * PI * f1 * t));
		rightdata = (short)(A * cos(2. * PI * f2 * t));
		data[i * 2] = leftdata; data[i * 2 + 1] = rightdata;
	}

	ofstream mywave("mywave.wav", ios::binary | ios::out);
	mywave.write((char*)&myHeader, sizeof(waveHeader));
	mywave.write((char*)data, sizeof(short) * n * myHeader.numChannels);
	mywave.close();
}

#define pi2 2 * 3.141592
void Harmony() {
	waveHeader wavHeader;
	short* data;
	float dt =  1/(float)(wavHeader.sampleRate); // sampleRate = 44100
	int n = wavHeader.sampleRate * 5; // n = 220500
	float f1 = 261.63, f2 = 329.63, f3 = 392.;
	data = new short[n * wavHeader.numChannels];
	float a1 = 10000., a2 = 15000., a3 = 20000.;
	for (int i = 0; i < n; i++) {
		float t = i * dt;
		data[i] = a1 * cos(pi2 * f1 * t) + a2 * cos(pi2 * f2 * t) + a3 * cos(pi2 * f3 * t);
	}
	ofstream mywave("mywave.wav", ios::binary | ios::out);
	mywave.write((char*)&wavHeader, sizeof(wavHeader)); // sizeof(wavHeader) = 44 Bytes44
	mywave.write((char*)data, sizeof(short) * n * wavHeader.numChannels);
}

void main() {
	Harmony();
}
