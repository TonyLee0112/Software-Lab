#include <iostream>
#include <fstream>
#include "bmpNew.h"
using namespace std;
int Drill1() {
	// Non - interlaced 방식 사용
// Bytes = (RGB)(RGB)(RGB)(RGB).. 
// 0~255 까지니까 1byte 짜리 unsigned char ptr 사용.
// signed char ptr = -127 ~ 128 -> 129 이상의 RGB 값 표현할 수 없다.
int width = 100; int length = 50;
unsigned char* data = new unsigned char[3 * width * length];

for (int i = 0; i < 3 * width * length; i+=3) {
	data[i] = 0; // B
	data[i + 1] = 0; // G
	data[i + 2] = 255; // R
}

WriteBmp("suhR.bmp",data,width,length);

// 이제 읽기
int x, y;
unsigned char* file = ReadBmp("suhR.bmp", &x, &y);
if (!file) {
	cout << "Cannot find image file" << endl;
}
cout << "Main " << x << " by " << y << endl;
}
void Drill2() {
	// 사전 정보
	int red_box_width = 100, red_box_length = 50;

	int width, length;
	unsigned char* sunflower = ReadBmp("sunflower.bmp", &width, &length);
	if (!sunflower) {
		cout << "Cant find file";
	}

	// Copy the original file
	unsigned char* tmp_bmp = new unsigned char[width * length * 3];
	for (int rep = 0; rep < width * length * 3; rep++) {
		tmp_bmp[rep] = sunflower[rep];
	}
	// 시작 주소 복사
	unsigned char* start;
	
	// 시작
	for (int y = 0; y < red_box_width; y++) {
		start = tmp_bmp + y * width * 3 + (width - red_box_width) * 3;
		for (int dx = 0; dx < red_box_width * 3; dx += 3) {
			start[dx] = 0;
			start[dx + 1] = 0;
			start[dx + 2] = 255;
		}
	}
	WriteBmp("New_flower2.bmp", tmp_bmp, width, length);
}
void Example1() {
	// Raw file 은 Header 가 없다.
	// 필요한 정보를 직접 추가해주어야한다.
	// bmp 와 다른 점: 시작점이 좌측 아래에서 좌측 위로 바뀜.
	// BGR 순서에서 RGB 순서로 바뀜
	int width = 200, length = 100;
	int dx;

	// 1. raw 파일 만들기
	unsigned char* test = new unsigned char[width * length * 3];
	for (int y = 0; y < length; y++) {
		int dy = y * width * 3;
		for (int rep = dx = 0; rep < width; rep++, dx += 3) {
			test[dy + dx] = 0;
			test[dy + dx + 1] = 0;
			test[dy + dx + 2] = 240;
		}
	}
	// ofstream 모드 : text 모드, binary 모드
	// binary 모드 -> 이미지, 음성, 영상 등등.. 데이터를 원본으로 읽고 쓰고 싶을 때
	// img file 을 다루기 때문에 binary 모드로
	ofstream raw("test.raw", ios::binary | ios::out);
	// char* 는 const char* 로 바로 cast 되지만 unsigned char* 는 다시 지정해주어야 함.
	raw.write((char*)test, width * length * 3);
	raw.close();

	// 2. 수정
	ifstream Raw("test.raw", ios::binary | ios::in);
	if (!Raw) {
		cout << "Oh my god";
		return;
	}
	Raw.read((char*)test, width * length * 3);
	Raw.close();

	unsigned char* a;
	for (int y = length / 2; y < length; y++) {
		a = test + y * width * 3;
		for (int dx = 0; dx < width; dx++) {
			a[dx * 3] = 255;
			a[dx * 3 + 1] = 0;
			a[dx * 3 + 2] = 0;
		}
	}

	ofstream raw_out("test2.raw", ios::binary | ios::out);
	raw_out.write((char*)test, width * length * 3);
	raw_out.close();
}
int Example2() {
	const int X = 500, Y = 300;
	unsigned char *yuv_image, *y, *u, *v;
	ofstream xx("test.yuv", ios::binary | ios::out);
	yuv_image = new unsigned char[X * Y * 3 / 2]; // 4 : 2: 0 방식
	y = yuv_image; // Luminance 명암
	u = y + X * Y; // Chromma U, Blue Difference
	v = u + X * Y / 4; // Chroma V, Red Difference
	for (int i = 0; i < Y; i++) {
		for (int j = 0; j < X; j++) {
			y[X * i + j] = 0;
		}
	}
	for (int i = 0; i < Y / 2; i++) {
		for (int j = 0; j < X / 2; j++) {
			u[i * X / 2 + j] = 0;
		}
	}
	for (int frames = 0; frames < 100; frames++) {
		for (int i = 0; i < Y / 2; i++) {
			if (i > frames * 5 && i < frames * 5 + 10) { // Red Bar range
				for (int j = 0; j < X / 2; j++) {
					v[i * X / 2 + j] = 255;
				}
			}
			else for (int j = 0; j < X / 2; j++) {
				v[i * X / 2 + j] = 0;
			}
		}
		xx.write((char*)yuv_image, X * Y * 3 / 2);
	}
	xx.close();
	return 123;
}
