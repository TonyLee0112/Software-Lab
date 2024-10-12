#include <iostream>
#include "bmpNew.h"
using namespace std;

void Affine_Transform(int x, int y, float* Matrix, int* new_x, int* new_y) {
	// Idk why we should return as ptr
	*new_x = (int)(x * Matrix[0] + y * Matrix[1]);
	*new_y = (int)(x * Matrix[2] + y * Matrix[3]);
}

int main() {
	// 1. Read a img file and Return at an image ptr
	int img_width, img_height;
	unsigned char* img = ReadBmp("sunflower.bmp", &img_width, &img_height); // Width : 564, Height : 374
	if (!img) {
		cout << "The image file does not exist in the same folder" << endl;
		return 666;
	}

	// 2. Make a room for new image file which is going to be Affine-transformed
	// Memory Allocation
	// The size for room = pixels * 3 bytes / pixel
	unsigned char* Transformed = new unsigned char[img_width * img_height * 3];

	// 3. Scaling, Rotating with Matrix
	float Affine_Matrix[4] = { 1,0,0,1 };
	for (int y = 0; y < img_height; y++) {
		for (int x = 0; x < img_width * 3; x += 3) {
			int newX, newY;

			// Translating
			Affine_Transform(x / 3 + img_width / 2, y + img_height / 2, Affine_Matrix, &newX, &newY);

			if (newX >= 0 && newX < img_width && newY >= 0 && newY < img_height) {
				int original_index = (y * img_width + x / 3) * 3;
				int transformed_index = (newY * img_width + newX) * 3;
				// BMP raw format - interlaced : (RGB) -> 1 pixel
				Transformed[transformed_index] = img[original_index]; // R
				Transformed[transformed_index + 1] = img[original_index + 1]; // G
				Transformed[transformed_index + 2] = img[original_index + 2]; // B
			}
		}
	}

	// 4. Write the new image
	WriteBmp("sunflower_modified.bmp", Transformed, img_width, img_height);
	return 123;
}
