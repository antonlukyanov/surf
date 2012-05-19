#ifndef IPOINT_H
#define IPOINT_H

#include <iostream>

using namespace std;

/**
 * Особая точка.
*/
class InterestPoint {
public:
    // Координаты найденной особой точки.
    float x, y;
    // Масштаб, в котором была найдена особая точка.
    float scale;
    // Направление особой точки.
    float orientation;
    // Дескриптор, состоящий из 64 элементов.
    float descriptor[64];
    // Знак лапласиана для определения на каком фоне была найдена точка.
    int laplacian;

    float operator-(const InterestPoint &p);
};

#endif // IPOINT_H
