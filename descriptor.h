#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#include "integralimage.h"
#include "interestpointtypes.h"
#include "misc.h"

using namespace std;

/**
 * Класс, занимающийся построением дескрипторов особых точек.
*/
class Descriptor {
public:
    float PI;
    Descriptor(IntegralImage *img, IPVect &ipts);
    // Запускает построение дескрипторов.
    void build();
private:
    IntegralImage *img;
    IPVect &ipts;
    // Нахождение ориентации i-й особой точки.
    void findOrientation(int pointIdx);
    // Построение дескриптора i-й особой точки.
    void buildDescriptor(int pointIdx);
    // Значение функции Гаусса в точке.
    float gaussian(int x, int y, float s);
    float gaussian(float x, float y, float s);
    // Вычисление фильтр Хаара в точке.
    float haarX(int row, int column, int s);
    float haarY(int row, int column, int s);
    // Находит угол в радианах для вектора (x, y).
    float getAngle(float x, float y);
};

#endif // DESCRIPTOR_H