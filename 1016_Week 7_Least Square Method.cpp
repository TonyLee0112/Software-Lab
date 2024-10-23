#include <iostream>
#include <Eigen/Dense>
#include <Eigen/LU>

using namespace std;
using namespace Eigen;

void Example1() {
	Matrix2d A;
	Vector2d B, C, D;
	A(0, 0) = 1.; A(0, 1) = 2;
	A(1, 0) = 3.; A(1, 1) = 4;

	Matrix2d Ai = A.inverse();

	cout << "[A]" << endl;
	cout << A << endl << endl;
	B[0] = 1; B[1] = 2;
	cout << "[B]" << endl;
	cout << B << endl << endl;

	C = A * B;
	cout << "[C]" << endl;
	cout << C << endl << endl;

	D = Ai * C;
	cout << "[D]" << endl;
	cout << D << endl << endl;
}
