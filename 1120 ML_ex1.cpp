#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

vector<double> input = { 2.,4.,6.,8. };
vector<double> target = { 4.,8.,12.,16. };

double forward(double x, double w);
double loss(double x, double y, double w);

int main() {
	vector<double> weights;
	vector<double> MSEs;

	for (double w = 0.0; w <= 4.0; w += 0.1) {
		cout << "w = " << w << endl;
		double I_sum = 0.0;

		for (size_t i = 0; i < input.size(); i++) {
			double x = input[i];
			double y = target[i];
			double pred = forward(x, w);
			double I = loss(x, y, w);
			I_sum += I;
			cout << "\t" << x << " " << y << " " << pred << " " << I << endl;
		}
		cout << "MSE= " << I_sum / input.size() << endl;
		weights.push_back(w);
		MSEs.push_back(I_sum / input.size());
	}
	return 0;
}

double forward(double x, double w) {
	return x * w;
}

double loss(double x, double y, double w) {
	double pred = forward(x, w);
	return pow(pred - y, 2);
}
