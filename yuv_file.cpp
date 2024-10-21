void Example2() {
	int X = 500, Y = 300;
	unsigned char* yuv_image, * y, * u, * v;
	ofstream file("test.yuv", ios::binary | ios::out);
	yuv_image = new unsigned char[X * Y * 3 / 2];
	// 각 색상 값 데이터의 시작 시점들을 알리는 ptr
	// 4 : 2 : 0
	y = yuv_image; // y -> XY
	u = y + X * Y; // u -> XY / 4
	v = u + X * Y / 4; // v -> XY / 4
	for (int i = 0; i < Y; i++) {
		for (int j = 0; j < X; j++) {
			// interlaced 방식
			y[i * X + j] = 0;
		}
	}

	for (int i = 0; i < Y / 2; i++) {
		for (int j = 0; j < X / 2; j++) {
			u[i * X / 2 + j] = 0;
		}
	}

	for (int frame = 0; frame < 100; frame++) {
		for (int i = 0; i < Y / 2; i++) {
			if (i > frame * 5 && i < frame * 5 + 10) {
				//
				for (int j = 0; j < X / 2; j++) {
					v[i * X / 2 + j] = 255;
				}
			}
			//
			else for (int j = 0; j < X / 2; j++) {
				v[i * X / 2 + j] = 0;
			}
		}
		file.write((char*)yuv_image, X * Y * 3 / 2);
	}
	file.close();
}
