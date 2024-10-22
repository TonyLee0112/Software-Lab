#include <iostream>
using namespace std;

void binary(float decimal) {
    // 주어진 소수(decimal)를 이진수로 변환하는 함수
    float remainder = decimal;
    float fraction = 0.5;
    int iteration = 0;
    cout << decimal << "\t Binary number in 20 digits: 0.";  // 20자리까지 이진수 출력

    while (remainder != 0 && iteration++ < 20) {
        if (remainder >= fraction) {
            // remainder가 fraction보다 크면 1을 출력
            cout << "1";
            // 나머지에서 현재 fraction을 빼줌
            remainder -= fraction;
        }
        else {
            // remainder가 fraction보다 작으면 0을 출력
            cout << "0";
            // fraction 값을 절반으로 줄여 더 작은 값으로 나눌 준비
            remainder *= 2;
        }
    }
    cout << endl;
}

void binary2(float decimal) {
    // 주어진 소수(decimal)를 이진수로 변환하는 함수
    float remainder = decimal;
    float fraction = 0.5;
    int iteration = 0;
    cout << decimal << "\t Binary number in 20 digits: 0.";  // 20자리까지 이진수 출력

    while (remainder != 0 && iteration++ < 20) {
        if (remainder >= fraction) {
            // remainder가 fraction보다 크면 1을 출력
            cout << "1";
            // 나머지에서 현재 fraction을 빼줌
            remainder -= fraction;
        }
        else {
            // remainder가 fraction보다 작으면 0을 출력
            cout << "0";
            // fraction 값을 절반으로 줄여 더 작은 값으로 나눌 준비
            fraction /= 2;
        }
    }
    cout << endl;
}
int main() {
    cout << "Version 1\n";
	binary(0.1);
    cout << "Version 2\n";
    binary2(0.1);
}


