#include <iostream>
#include <Eigen/Dense>
#include <cmath>
#include "bmpNew.h"
#define PI 3.141592

using namespace Eigen;
using namespace std;

void Affine_Transform_image(Matrix2d Matrix, int x, int y, int* newX, int* newY) {
    // 변수를 ptr 로 쓰면 장점 : 반환을 할 필요가 없다!
    Vector2d original(x, y); // 원래 좌표
    Vector2d transformed = Matrix * original;
    *newX = (int)(transformed[0]);
    *newY = (int)(transformed[1]);
}

void Affine_inverse_Transform_image() {

}

Matrix2d get_Matrix_A_2D(float a, float b, float x) {
    // This Matrix gives a Matrix A for Scaling and Rotation
    // size of Matrix A = 2X2
   
    // scaling matrix S
    Matrix2d S;
    S << a, 0, 0, b;

    // Rotating Matrix R
    Matrix2d R;
    float degree_to_rad = PI / 180. ;
    R << cos(x * degree_to_rad), -sin(x * degree_to_rad), sin(x * degree_to_rad), cos(x * degree_to_rad);

    // R * S : 회전 후 스케일링
    Matrix2d A;
    A = R * S;

    return A;
}

int main() {
    // 1. 이미지 파일 읽기
    int img_width, img_height;
    unsigned char* img = ReadBmp("sunflower.bmp", &img_width, &img_height);
    if (!img) {
        cout << "Cannot find the file" << endl;
        return 666;
    }

    // 2. 변환될 이미지를 위한 공간 할당 Memory Allocation    
    // unsigned char ptr 쓰는 이유 : 1 byte 단위라서. RGB 각각 1 byte 자너
    // int ptr 쓰게 되면 모든 원소들이 4 bytes 씩으로 변환돼서 공간을 더 많이 차지함. RGB 1 pixel = 12 bytes
    unsigned char* Transformed = new unsigned char[img_width * img_height * 3];

    // 3. Affine 변환 행렬 생성
    Matrix2d Affine_Matrix = get_Matrix_A_2D(2,1./2,30);
    //Affine_Matrix << 1, 0, 0, 1; // 2X2

    // 4. Affine 변환
    // Bmp file 이미지 시작점 : 왼쪽 하단 -> (0,0)
    for (int y = 0; y < img_height; y++) {
        // 1 row 의 길이 = img_width * 3 bytes / pixel
        int width_bytes = img_width * 3;
        for (int x = 0; x < width_bytes; x += 3) {
            int newX, newY;
            // Translating
            // x / 3 이 실제 x 좌표.
            Affine_Transform_image(Affine_Matrix, x/3 , y, &newX, &newY); // &newX : newX 의 주소, (x/3,y) 가 변환되어서 (newX,newY) 에 저장됨.

            if (newX >= 0 && newX < img_width && newY >= 0 && newY < img_height) {
                // 두 index 모두 byte 단위
                int original_index = y * width_bytes + x; // x grows 3 bytes every time
                int transformed_index = newY * width_bytes + newX * 3; // newX is not a byte address, but a coordinate : (newX,newY)

                // Bmp raw format - interlaced : (RGB) -> 1 pixel
                Transformed[transformed_index] = img[original_index];
                Transformed[transformed_index + 1] = img[original_index + 1];
                Transformed[transformed_index + 2] = img[original_index + 2];
            }
        }
    }
    WriteBmp("sunflower_Eigened.bmp", Transformed, img_width, img_height);

    delete[] img; // 메모리 해제
    delete[] Transformed; // 메모리 해제
    return 123;
}
