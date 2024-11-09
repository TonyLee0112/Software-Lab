#include <iostream>
using namespace std;

int example1() {
	// Uniform distribution
	int hits[6];
	int Ntry = 10000;
	cout << " RAND_MAX = " << RAND_MAX << endl;
	for (int i = 0; i < 6; i++) {
		hits[i] = 0;
	}
	for (int n = 0; n < Ntry; n++) {
		hits[rand() % 6]++;
	}

	for (int i = 0; i < 6; i++) {
		cout << i + 1 << " " << (float)hits[i] / Ntry << endl;
	}

	return 123;
}
int Example2() {
	// Monte-Carlo Method : Non-uniform distribution
	// 0~100 의 확률 구간을 분할 -> 구간 길이들을 설정
	float pdf[] = { 12.3,26.5,17.23,22.3,9.2,12.47 };
	float cdf[6];
	cdf[0] = pdf[0];
	for (int i = 1; i < 6; i++) {
		cdf[i] = cdf[i - 1] + pdf[i];
	}
	int hits[6];
	for (int ii = 0; ii < 6; ii++) {
		hits[ii] = 0;
	}
	int Ntry = 10000;
	for (int n = 0; n < Ntry; n++) {
		float p = (float)rand() / RAND_MAX * 100.;
		// 랜덤 추출된 확률값이 속하는 구간에 출현 횟수 +1
		for (int id = 0; id < 6; id++) {
			if (p < cdf[id]) {
				hits[id]++;
				break;
			}
		}
	}
	for (int i = 0; i < 6; i++) {
		cout << i + 1 << " " << (float)hits[i] / Ntry * 100. << endl;
	}
	return 123;
}
