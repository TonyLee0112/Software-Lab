// 내 생각엔 Affine_Transform 에서 2차원이나 3차원 Matrix 로 Affine_Matrix 사용하는데, Eigen Library 를 써서 하지는 않았다.
// 그러니까 같이 사용하는 문제도 나오지 않을까?
// 같이 사용하는 코드를 ChatGPT 로 만들어보았다.
// 조금 더 코드 정리가 필요하긴 함.

#include <iostream>
#include <Eigen/Dense>
#include "bmpNew.h"
using namespace std;
using namespace Eigen;

void Affine_Transform(int x, int y, const Matrix2d& Matrix, int* new_x, int* new_y) {
    // x와 y는 원래 좌표, Matrix는 변환 행렬
    Vector2d original(x, y);  // 2D 벡터로 원래 좌표를 표현
    Vector2d transformed = Matrix * original;  // Affine 변환 수행
    *new_x = static_cast<int>(transformed[0]);
    *new_y = static_cast<int>(transformed[1]);
}

int main() {
    // 1. 이미지 파일을 읽어서 포인터로 받음
    int img_width, img_height;
    unsigned char* img = ReadBmp("sunflower.bmp", &img_width, &img_height); // 너비: 564, 높이: 374
    if (!img) {
        cout << "The image file does not exist in the same folder" << endl;
        return 666;
    }

    // 2. 변환된 이미지를 위한 공간 할당 (픽셀 * 3 바이트/픽셀)
    unsigned char* Transformed = new unsigned char[img_width * img_height * 3];

    // 3. Affine 변환 행렬 (Identity Matrix로 시작)
    Matrix2d Affine_Matrix;
    Affine_Matrix << 1, 0,
                     0, 1;  // 기본적으로 아무런 변환도 하지 않음 (Identity Matrix)

    for (int y = 0; y < img_height; y++) {
        for (int x = 0; x < img_width * 3; x += 3) {
            int newX, newY;

            // 원래 이미지 좌표를 변환된 좌표로 변환
            Affine_Transform(x / 3 + img_width / 2, y + img_height / 2, Affine_Matrix, &newX, &newY);

            // 변환된 좌표가 유효한 범위 내에 있는지 확인
            if (newX >= 0 && newX < img_width && newY >= 0 && newY < img_height) {
                int original_index = (y * img_width + x / 3) * 3;
                int transformed_index = (newY * img_width + newX) * 3;

                // BMP raw 포맷에서 RGB 값을 복사
                Transformed[transformed_index] = img[original_index]; // R
                Transformed[transformed_index + 1] = img[original_index + 1]; // G
                Transformed[transformed_index + 2] = img[original_index + 2]; // B
            }
        }
    }

    // 4. 변환된 이미지를 새로운 파일로 저장
    WriteBmp("sunflower_modified_eigen.bmp", Transformed, img_width, img_height);
    return 123;
}
