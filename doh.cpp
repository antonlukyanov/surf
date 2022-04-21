#include "doh.h"

DoH::DoH(int width, int height, int step, int filtersize) {
    this->width = width;
    this->height = height;
    this->step = step;
    this->filtersize = filtersize;

    determinant = new float[width * height];
    laplacian = new unsigned char[width * height];

    memset(determinant, 0, sizeof(float) * width * height);
    memset(laplacian, 0, sizeof(unsigned char) * width * height);
}

DoH::~DoH() {
    if (determinant) {
        delete[] determinant;
    }

    if (laplacian) {
        delete[] laplacian;
    }
}

byte DoH::getLaplacian(uint row, uint column) {
    return laplacian[row * width + column];
}

byte DoH::getLaplacian(uint row, uint column, DoH *src) {
    int scale = this->width / src->width;
    return laplacian[(scale * row) * width + (scale * column)];
}

float DoH::getDet(uint row, uint column) {
    return determinant[row * width + column];
}

float DoH::getDet(uint row, uint column, DoH *src) {
    int scale = this->width / src->width;
    return determinant[(scale * row) * width + (scale * column)];
}
