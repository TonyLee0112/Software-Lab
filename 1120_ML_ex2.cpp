#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

vector<double> train_input = { 2.0,4.0,6.0,8.0 };
vector<double> train_target = { 4.0,8.0,12.0,16.0 };

double w = 1.0;
double learning_rate = 0.01;

double FWD_propagation(double x) {
	return x * w;
}

double Loss(double x, double y) {
	double predict = FWD_propagation(x);
	double e = y - predict;
	return e * e;
}

double Gradient(double x, double y) {
	// (xTw - y)^2 를 w 로 미분
	// dL/dw
	return 2 * x * (x * w - y);
}

int main() {
	cout << "Prediction (Before Training): " << FWD_propagation(10) << endl;
	cout << endl;

	// Training
	for (int epoch = 0; epoch < 10; epoch++) {
		double loss_sum = 0.0; // Initialize loss for each epoch
		cout << "Epoch : " << epoch << endl;
		for (size_t i = 0; i < train_input.size(); i++) {
			double x = train_input[i];
			double y = train_target[i];

			double grad = Gradient(x, y);
			w -= learning_rate * grad;
			cout << "Loss function Gradient at (" << x << ", " << y << ") : " << grad << endl;
			loss_sum += Loss(x, y);
		}
		cout << "Updated weight = " << w << endl;
		cout << "Sum of Loss = " << loss_sum << endl;
		cout << endl;
	}

	// After Training
	cout << "Post-training prediction for input : 10 -> " << FWD_propagation(10) << endl;
	return 0;
}
