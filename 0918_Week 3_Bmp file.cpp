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
int Example1() {
	// How to make a raw image file
	// 1. write a raw file
	int x = 200, y = 100;
	int dx;
	int x3 = 600; // One Horizontal line bytes
	unsigned char* test;
	test = new unsigned char[x * y * 3];
	for (int i = 0; i < y; i++) {
		// Raw file starts at the top
		int xx = i * x3; // i th floor
		for (int Rep = dx = 0; Rep < x; Rep++, dx += 3) {
			test[xx + dx] = 0; // R
			test[xx + dx + 1] = 0; // G
			test[xx + dx + 2] = 240; // B
		}
	}
	// Now we have a series of RGB image raw data
	// Let's make a image file with this
	ofstream raw("test.raw", ios::binary | ios::out);
	raw.write((char*)test, x * y * 3); // Input : Location, Data size
	raw.close();

	// 2. Modify
	ifstream Raw("test.raw", ios::binary | ios::in); // 위에서 close 하더라도 object name 이 겹치면 안됨. 오류뜸
	if (!Raw) {
		cout << "Can't Read\n"; return 666;
	}
	Raw.read((char*)test, x * y * 3); // raw 했다가 오류 뜸
	Raw.close();
	for (int floor = y / 2; floor < y; floor++) {
		int xx = floor * x3;
		for (int Rep = dx = 0; Rep < x; Rep++, dx += 3) {
			test[xx + dx] = 255; // R
			test[xx + dx + 1] = 0; // G
			test[xx + dx + 2] = 0; // B
		}
	}

	// 3. Write the modified image
	ofstream Raw_out("test2.raw", ios::binary | ios::out);
	Raw_out.write((char*)test, x * y * 3);
	Raw_out.close();
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
