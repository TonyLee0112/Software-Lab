#include <iostream>
#include <cmath>
using namespace std;
const int Ntry = 10000;

class discreteRV {
	// 클래스는 함수를 합쳐놓을 수 있다
public:
	discreteRV(){} // Initializer
	discreteRV(int n, int l, int h) {
		init(n, l, h);
	}
	void init(int n, int l, int h); // Class 에서는 함수를 나중에 써도 되네
	int N;
	int xlow, xhigh;
	float* pdf;
	float Ex, Var;
	void calculateExVar();
	// virtual : 파생 class 에서도 사용가능하도록
	virtual void experiment() {};
};
// 함수를 쓸 때는 ::, class 정의할 때는 :
class dice : public discreteRV {
public :
	dice() {};
	dice(int n, int l, int h) {
		init(n, l, h);
		experiment(); // 이거 왜 함?
	}
	void experiment();
};
void discreteRV::init(int n, int l, int h) {
	N = n; xlow = l; xhigh = h;
	Ex = Var = 0;
	pdf = new float[N];
	for (int i = 0; i < N; i++) pdf[i] = 0.;
}
void discreteRV::calculateExVar() {
	// 이론상 평균, 분산
	float Ex2 = 0.;
	Ex = 0.;
	for (int i = 0; i < N; i++) {
		Ex += pdf[i] * (xlow + i);
		Ex2 += pdf[i] * pow(xlow + i, 2);
	}
	Var = Ex2 - pow(Ex, 2);
}
void dice::experiment() { 
	// 통계적 pdf, 평균, 분산
	for (int i = 0; i < Ntry; i++) {
		int ix = rand() % 6; // 0~5
		float x = ix + 1;
		Ex += x;
		Var += pow(x, 2);
		pdf[ix]++;
	}
	Ex /= Ntry;
	Var = Var / Ntry - pow(Ex, 2);
	for (int i = 0; i < N; i++) {
		pdf[i] /= Ntry;
	}
}

int main() {
	// Get Mean & Variance by using PDF
	dice mydice(6, 1, 6); // 시행횟수, xlow, xhigh

	mydice.experiment();
	cout << "  통계적 계산" << endl;
	cout << "  dice Expectation " << mydice.Ex << endl;
	cout << "  dice Variance " << mydice.Var << endl;
	cout << "  P[X] " << mydice.pdf[0] << endl;

	mydice.calculateExVar();
	cout << "  이론적 계산 " << endl;
	cout << "  dice Expectation " << mydice.Ex << endl;
	cout << "  dice Variance " << mydice.Var << endl;
	return 123;
}
