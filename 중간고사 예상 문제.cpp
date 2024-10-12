// 내 생각엔 Affine_Transform 에서 2차원이나 3차원 Matrix 로 Affine_Matrix 사용하는데, Eigen Library 를 써서 하지는 않았다.
// 그러니까 같이 사용하는 문제도 나오지 않을까?
// 같이 사용하는 코드를 ChatGPT 로 만들어보았다.
// 조금 더 코드 정리가 필요하긴 함.

#include <iostream>
#include <Eigen/Dense>
#include "bmpNew.h"

using namespace Eigen;
using namespace std;

void affineXform(const Matrix3d& transformMatrix, int inputX, int inputY, int& outputX, int& outputY) {
    // 3D homogeneous coordinates (x, y, 1)
    Vector3d inputVector(inputX, inputY, 1);
    
    // Apply the affine transformation
    Vector3d outputVector = transformMatrix * inputVector;
    
    // Extract the transformed X and Y coordinates
    outputX = static_cast<int>(outputVector[0]);
    outputY = static_cast<int>(outputVector[1]);
}

int main() {
    // 1. Read a BMP file
    int width, height;
    unsigned char* originalImage = ReadBmp("sunflower.bmp", &width, &height); // 24-bit BMP file
    if (!originalImage) { // If the file does not exist
        std::cout << "Cannot find image file" << std::endl;
        return 666;
    }

    std::cout << "Width: " << width << " Height: " << height << std::endl;

    // 2. Allocate memory for the modified image
    unsigned char* transformedImage = new unsigned char[width * height * 3];

    // 3-1. Define the affine transformation matrix (Identity matrix with translation)
    Matrix3d affineMatrix;
    affineMatrix << 1, 0, 0,  // No scaling, no rotation
                    0, 1, 0,  // No scaling, no rotation
                    0, 0, 1;  // No translation

    // 3-2. Apply the transformation to each pixel
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width * 3; x += 3) { // Step by 3 to account for R, G, B
            int newX, newY;
            affineXform(affineMatrix, x / 3 - width / 2, y - height / 2, newX, newY); // Transform pixel positions

            // Check if the new coordinates are within bounds
            if (newX >= 0 && newX < width && newY >= 0 && newY < height) {
                int transformedIndex = (newY * width + newX) * 3;
                int originalIndex = (y * width + x / 3) * 3;

                // Copy RGB values from the original image to the transformed image
                transformedImage[transformedIndex] = originalImage[originalIndex];
                transformedImage[transformedIndex + 1] = originalImage[originalIndex + 1];
                transformedImage[transformedIndex + 2] = originalImage[originalIndex + 2];
            }
        }
    }

    // 4-1. Write the modified image to a new BMP file
    WriteBmp("sunflowerTransformedEigen.bmp", transformedImage, width, height);
    return 123;
}
