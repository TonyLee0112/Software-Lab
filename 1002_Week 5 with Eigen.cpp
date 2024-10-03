#include <iostream>
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

void Example1() {
	Matrix2d a;
	a << 1, 2,
		 3, 4;
	MatrixXd b(2, 2);
	b << 2, 3,
		1, 4;

	cout << "a + b =\n" << a + b << endl;
	cout << "a - b =\n" << a - b << endl;
	cout << "Doing a += b" << endl;
	a += b;
	cout << "Now a =\n" << a << endl;
	Vector3d v(1, 2, 3);
	Vector3d w(1, 0, 0);
	cout << "-v + w - v = \n" << -v + w - v << endl;
}

void Example2() {
	MatrixXd d34(3, 4);
	d34 << 2, 3, 4, 5,
		0, -1, 1, 1,
		1, 4, 0, 0;
	cout << " d " << endl << d34 << endl;

	MatrixXd e43(4, 3);
	e43 << 2, 3, 4,
		5, 0, -1,
		1, 1, 1,
		4, 0, 0;

	MatrixXd f = d34 * e43;
	cout << "e43\n" << e43 << endl << " f\n" << f << endl;
}

void Example3() {
	VectorXd* xX = new VectorXd[10];
	for (int i = 0; i < 10; i++) {
		xX[i] = VectorXd(4); // Set 4th Dimension
		for (int j = 0; j < 4; j++) {
			xX[i][j] = i + j;
			cout << "\n\n" << xX[i] << endl;
		}
	}

	// orthogonal
	xX[0] << 0.5, 0.5, 0.5, 0.5;
	xX[1] << 0.5, 0.5, -0.5, -0.5;
	xX[2] << 0.5, -0.5, -0.5, 0.5;
	xX[3] << 0.5, -0.5, 0.5, -0.5;
	cout << endl;
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cout << xX[i].dot(xX[j]) << " ";
		}
		cout << endl;
	}
}

void Example4() {
	Matrix3d m = Matrix3d::Random();
	cout << "Here is the Matrix m : " << endl << m << endl;
	cout << "Its inverse is : " << endl << m.inverse() << endl;
}
