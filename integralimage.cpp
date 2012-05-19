#include "integralimage.h"
#include "bitmap.h"

IntegralImage::IntegralImage(Bitmap &b) : bitmap(b) {
    float rs;
    RGBTriple p;

    data = new float*[bitmap.getHeight()];

    for ( long i = 0; i < bitmap.getHeight(); ++i ) {
        data[i] = new float[bitmap.getWidth()];
        memset(data[i], 0, bitmap.getWidth());

        rs = 0;
        for ( long j = 0; j < bitmap.getWidth(); ++j ) {
            bitmap.at(i, j, p);
            rs += (float)p.r / 255.0;
            data[i][j] = rs + (i != 0 ? data[i - 1][j] : 0.0);
        }
    }

    width = b.getWidth();
    height = b.getHeight();
}

IntegralImage::~IntegralImage() {
    for ( long i = 0; i < bitmap.getHeight(); ++i ) {
        delete[] data[i];
    }

    delete[] data;
}

float IntegralImage::sum(long row, long col, long rows, long cols) {
    long r1 = min(row, bitmap.getHeight()) - 1,
         c1 = min(col, bitmap.getWidth())  - 1,
         r2 = min(row + rows, bitmap.getHeight()) - 1,
         c2 = min(col + cols, bitmap.getWidth())  - 1;

    float A = 0,
          B = 0,
          C = 0,
          D = 0;

    if (r1 >= 0 && c1 >= 0) {
        A = data[r1][c1];
    }

    if (r1 >= 0 && c2 >= 0) {
        B = data[r1][c2];
    }

    if (r2 >= 0 && c2 >= 0) {
        C = data[r2][c2];
    }

    if (r2 >= 0 && c1 >= 0) {
        D = data[r2][c1];
    }

    return max(0.f, A - B - D + C);
}

float IntegralImage::at(long i, long j) {
    return data[i][j];
}

void IntegralImage::setBitmap(Bitmap &b) {
    bitmap = b;
}