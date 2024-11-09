#include <iostream>
#include <fstream>
#include <random>
using namespace std;

const int N = 10000;
const float m = 5.0, s = 2.0;

int grid(float x) {
	// return normalized x
	int z = (x - m) / s; // -2s ~ 2s : 데이터의 95% 
	int z_shifted = z + 2; // 양수가 다루기 편하니까 0s ~ 4s 로 전환 
	int i = z / 4. * 100; // shift된 z 값을 0~99 사이의 index 로 전환
	if (i > 100) i = 99; else if (i <= 0) i = 0;
	return i;
}

int main() {
	// Example 7
	// Gaussian simulation
	default_random_engine generator;
	normal_distribution<float> Gauss(m, s);
	float hit[100];
	for (int i = 0; i < 100; i++) hit[i] = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			float random_number = Gauss(generator);
			int normalized_x = grid(random_number); // 0~99
			hit[normalized_x]++;
		}
	}
	ofstream file("Gaussian_Simulation.txt");
	for (int i = 0; i < 100; i++) {
		// grid 에서 변환한 과정의 역과정 -> x 복원
		file << ((float)i / 100. * 4. - 2.) * s + m << " " << hit[i] << endl;
	}
	file.close();
	return 1;
}
