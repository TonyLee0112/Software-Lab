#include <iostream>
using namespace std;
const int trials = 10000;
int example1() {
	// Uniform distribution
	int hits[6];
	int trials = 10000;

	cout << " RAND_MAX 변수값 : " << RAND_MAX << endl;
	for (int i = 0; i < 6; i++) {
		// Initialize
		hits[i] = 0;
	}

	for (int n = 0; n < trials; n++) {
		// Random trials
		hits[rand() % 6]++;
	}
	for (int i = 0; i < 6; i++) {
		// 각 칸의 히트 수를 총 시행횟수로 나눠주면 전체에 대한 해당 칸의 히트 비율 = 확률
		cout << i + 1 << " " << (float)hits[i] / trials << endl;
	}
	return 123;
}
int example2() {
	// Monte-Carlo Method : Non-uniform distribution
	// 0~100 까지의 확률 구간을 6개의 구간으로 분할 -> 구간 길이들을 설정.
	float pdf[] = { 12.3,26.5,17.23,22.3,9.2,12.47 };
	float cdf[6];

	// Replicate the first element of PDF into CDF
	cdf[0] = pdf[0];

	// Set CDF
	for (int i = 1; i < 6; i++) {
		cdf[i] = cdf[i - 1] + pdf[i];
	}

	// Prepare Probability space
	int hits[6];
	for (int s = 0; s < 6; s++) {
		hits[s] = 0;
	}

	int trials = 10000;

	for (int n = 0; n < trials; n++) {
		// rand() 로 값 추출하면 0~RAND_MAX 나오니까 0~100 으로 Normalize
		float p = (float)rand() / RAND_MAX * 100.;

		for (int idx = 0; idx < 6; idx++) {
			if (p < cdf[idx]) {
				// 랜덤추출된 0~100 사이의 값이 구간에 속하면 히트
				hits[idx]++;
				break;
			}
		}
	}

	for (int i = 0; i < 6; i++) {
		// 구간별 히트 수를 확률로 변환
		cout << i + 1 << " " << (float)hits[i] / trials * 100. << endl;
	}
	return 123;

}

class discreteRV {
public:
	discreteRV() {}
	discreteRV(int n, int l, int h) {
		init(n, l, h);
	}
	void init(int n, int l, int h);
	int N; // Number of elements -> 6
	int x_min, x_max;
	float* pdf;
	float Ex, Var;
	void CalculateExVar();
	virtual void experiment() {};
};
void discreteRV::init(int n, int l, int h) {
	N = n; // 주사위 눈의 수 6
	x_min = l; // 주사위 눈 1
	x_max = h; // 주사위 눈 6
	pdf = new float[N]; // 각각의 주사위 눈이 나올 확률질량함수
	Ex = Var = 0; // Statistical value initialize
	for (int i = 0; i < N; i++) pdf[i] = 0.0;
}
class dice : public discreteRV {
public:
	// 1회의 주사위 던지는 시행하기
	dice() {};
	dice(int n, int l, int h) {
		init(n, l, h);
		experiment();
	}
	void experiment();
};
void dice::experiment() {
	// trials 번 주사위를 던지는 시행
	// 실험적으로 PDF, Mean, Variance 계산
	for (int i = 0; i < trials; i++) {
		int idx = rand() % 6; // 0~5
		float x = idx + 1; // 1~6 의 확률변수 값으로 shift
		Ex += x;
		Var += x * x;
		pdf[idx]++;
	}
	Ex /= trials; // 모든 변량의 합을 평균내기
	Var = Var / trials - Ex * Ex;

	for (int i = 0; i < N; i++) {
		pdf[i] /= trials;
	}

}
void discreteRV::CalculateExVar() {
	// Theoretical Statistics

	float EX2 = 0.;
	Ex = 0.;
	for (int i = 0; i < N; i++) {
		// x_min = 1, x_max = 6
		Ex += pdf[i] * (x_min + i);
		EX2 += pdf[i] * (x_min + i) * (x_min + i);
	}
	Var = EX2 - Ex * Ex;
}
int example3() {
	dice mydice(6, 1, 6);

	// 실험으로 통계량 계산
	mydice.experiment();
	cout << " Experimental Expected value : " << mydice.Ex << endl;
	cout << " Experimental Variance : " << mydice.Var << endl;
	cout << " Experimental PDF P[x] : " << mydice.pdf[0] << endl;

	// 이론적으로 통계량 계산
	mydice.CalculateExVar();
	cout << " Theoretical Expected value : " << mydice.Ex << endl;
	cout << " Theoretical Variance : " << mydice.Var << endl;
	
	return 123;
}



#include <cmath>
const int n = 10, N = 10000;
const float p = 1. / 6.;
float factorial(float x) {
	if (x == 0 || x == 1.0) return 1.0;
	return x * factorial(x - 1);
}
float calc_PMF(float p, int x, int n) {
	// n 회의 독립시행 중에 x 번 p의 확률로 성공할 확률
	return pow(p, x) * pow(1. - p, n - x) * factorial(n) / factorial(x) / factorial(n - x);
}
void example4() {
	// Experimental Result
	// 이항분포
	// 표본집단 : X_bar = (X1 + X2 + ... + Xn) / n
	// N 개의 표본집단에 대한 통계량
	float PX[n + 1], mean = 0, Var = 0.;

	// PMF initialization
	for (int x = 0; x <= n; x++) PX[x] = 0.;

	// N set ( 1 set implemenation = n - independent trials)
	for (int i = 0; i < N; i++) {
		int success = 0;
		for (int j = 0; j < n; j++) {
			// n 번 중에 1/6 미만의 확률이 몇번이나 나오는지 (= 성공) cnt
			if ((float)rand() / (float)RAND_MAX < p) success++;
		}
		// 각각의 성공횟수들이 나온 횟수를 count
		PX[success] += 1.;
		mean += success;
		Var += pow(success, 2);
	}
	for (int x = 0; x <= n; x++) {
		// 성공횟수 / 총 횟수 = 확률
		PX[x] /= (float)N;
	}
	mean /= (float)N;
	Var /= (float)N; Var -= mean * mean;

	// Theoretical Result
	cout << " mean " << mean << " " << n * p << endl;
	cout << " Var " << Var << " " << n * p * (1 - p) << endl << endl << " prob. \n";
	for (int x = 0; x < n + 1; x++) {
		cout << " " << x << " " << PX[x] << " " << calc_PMF(p, x, n) << "\n";
	}
	return;
}


#include <random>
void example6() {
	default_random_engine generator;
	normal_distribution<double> myGauss(5.0, 2.0);
	double EX = 0, Var = 0;
	int N = 10000;
	for (int i = 0; i < N; i++) {
		double Random_num = myGauss(generator);
		EX += Random_num;
		Var += Random_num * Random_num;
	}
	EX /= (double)N;
	Var /= (double)N;
	Var -= EX * EX;
	cout << " mean " << EX << " " << "5.0\n";
	cout << " Var " << Var << " " << "4.0\n";
}

#include <fstream>
const int NN = 10000;
const float m = 5.0, s = 2.0;
int grid(float x) {
	// Transform random value x to index among range 0~99
	// Normalization
	float z = (x - m) / s;
	// -2s ~ 2s will be treated
	// Shift to positive region : 0s ~ 4s
	int z_positive = z + 2;

	// Normalize again
	int z_p_norm = z_positive / 4;

	// Make it index
	int idx = z_p_norm * 100;

	if (idx > 100) {
		idx = 99;
	}
	else if (idx <= 0) {
		idx = 0;
	}
	return idx;
}
void example7() {
	// Gaussian Simulation
	default_random_engine generator;
	normal_distribution<float> Gauss(m, s);
	
	// 결과 저장용 Array 생성 및 초기화
	float hit[100];
	for (int i = 0; i < 100; i++) hit[i] = 0;

	// N 개의 랜덤값 생성
	for (int j = 0; j < NN*NN; j++) {
		float rand_num = Gauss(generator);
		int rand_idx = grid(rand_num);
		hit[rand_idx]++;
	}

	ofstream file("graph.txt");
	for (int i = 0; i < 100; i++) {
		// De-Normalization 후 Write
		file << ((float)i / 100. * 4 - 2.0) * s + m << " " << hit[i] << endl;
	}
	file.close();
}

void main() {
	example7();
}
