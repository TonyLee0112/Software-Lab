#include <Eigen/Dense>
#include <Eigen/LU>
#include <iostream>
using namespace std;
using namespace Eigen;

void Example1();
void Example3();
void Drill1();

void main(){}

void Example1() {
	MatrixXd A(3, 3);
	A << 2, 0, -2,
		1, 1, -2,
		0, 0, 1;

	// Matrix A 의 Eigenvalue 와 Eigenvector 계산
	// eigenvalues 는 complex format 으로 나온다. (Real,Imaginary)
	EigenSolver<MatrixXd> es;
	es.compute(A);
	cout << "The eigenvalues (Re,Im) of A are : " << endl << es.eigenvalues() << endl;

	// P for Matrix Decomposition
	// A = P Sigma P^-1
	MatrixXd P(3, 3);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			P(i, j) = es.eigenvectors()(i, j).real();
		}
	}
	cout << "\n\nP\n" << P << endl;

	// Sigma 에 해당하는 Matrix S
	// S = Eigenvalue 들의 Diagonal Matrix
	MatrixXd S(3, 3);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			S(i, i) = es.eigenvalues()[i].real();
		}
	}
	// Pi = P_inverse
	MatrixXd PI = P.inverse();

	cout << "\n\nA = P S PI \n" << P * S * PI << endl;
}

void Example3() {
	// A -> det(A- eigenvalue * I) = 0 -> P = [v1,v2,v3] -> Normalized P = [v1/|v1| v2/|v2| v3/|v3|]
	// Normalized P -> P inverse = P transpose
	MatrixXd P(3, 3);
	P << 0, 2 / sqrt(6), 1 / sqrt(3),
		1 / sqrt(2), -1 / sqrt(6), 1 / sqrt(3),
		-1 / sqrt(2), -1 / sqrt(6), 1 / sqrt(3);

	MatrixXd Pi = P.inverse();

	// Eigenvalue 들을 Diagonalize
	MatrixXd S(3, 3);
	S << 0, 0, 0,
		0, 2, 0,
		0, 0, 5;

	cout << "P S Pi\n" << P * S * Pi << endl;
}

void Drill1() {
	// Diagonalization
	MatrixXd A(2, 2);
	A << 7, 2,
		-4, 1;
	// Calculate A^k
	// A^k = PDPi * PDPi * PDPi * ... * PDPi = P* D^k * Pi
	// 계산량을 엄청 줄일 수 있다는 장점

	MatrixXd P(2, 2);
	EigenSolver<MatrixXd> es;
	es.compute(A);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			P(i, j) = es.eigenvectors()(i, j).real();
		}
	}
	MatrixXd D(2, 2);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			D(i, i) = es.eigenvalues()[i].real();
		}
	}

	MatrixXd Pi = P.inverse();

	int k = 3;
	for (int i = 1; i < k; i++) {
		A *= A;
		D *= D;
	}

	cout << "k= " << k << endl;
	cout << "\nA^k\n" << A << endl;
	cout << "\n(PDPi)^k\n" << P * D * Pi << endl;
}
