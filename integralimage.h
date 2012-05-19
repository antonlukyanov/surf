#ifndef INTEGRAL_IMAGE_H
#define INTEGRAL_IMAGE_H

#include "bitmap.h"

using namespace std;

/**
 * »нтегральное представление изображени€.
*/
class IntegralImage {
public:
    IntegralImage(Bitmap &b);
    ~IntegralImage();
    float at(long i, long j);
    float sum(long row, long col, long rows, long cols);
    void setBitmap(Bitmap &b);

    long width, height;
private:
    float **data;
    Bitmap &bitmap;
};

#endif // INTEGRAL_IMAGE_H
