#ifndef DoH_H
#define DoH_H

#include <iostream>
#include "types.h"

using namespace std;

class DoH {
public:
    int width, height;
    int step;
    int filtersize;

    byte *laplacian;
    float *determinant;

    DoH(int width, int height, int step, int filtersize);
    ~DoH();

    byte getLaplacian(uint row, uint column);
    byte getLaplacian(uint row, uint column, DoH *src);
    float getDet(uint row, uint column);
    float getDet(uint row, uint column, DoH *src);
};

#endif // DoH_H
