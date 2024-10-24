#include <iostream>
#include <fstream>
using namespace std;
#define PI 3.141592

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

// Create wave file that produces "C" note at left speaker and "E" note at right speaker for 5 seconds
// 1. Stereo 써야겠다.
void Example2() {
	short* data; // short = 2 bytes(16 bits) -> 이 값 범위로 sinusoidal 값 다 표현
	short rightdata, leftdata;
	waveHeader myHeader;
	int n = myHeader.sampleRate * 5; // 5 초 = 44100 * 5 개의 note data 가지고 있어야 됨.

	data = new short[n * myHeader.numChannels]; // numChannels = 2 -> 좌우 따로 해서 두 배의 데이터를 들고 있어야 함.
	float dt = 1.0 / (float)(myHeader.sampleRate);
	float f_note_C = 261.63, f_note_E = 329.63, Amplitude = 10000.0;

	for (int i = 0; i < n; i++) {
		// n : 재생시간 5초 동안 총 data 수
		float t = i * dt;

		leftdata = (short)(Amplitude * cos(2.0 * PI * f_note_C * t));
		rightdata = (short)(Amplitude * cos(2.0 * PI * f_note_E * t));
		data[i * 2] = leftdata;
		data[i * 2 + 1] = rightdata;
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
