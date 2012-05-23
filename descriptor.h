#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#include "integralimage.h"
#include "interestpointtypes.h"
#include "misc.h"

using namespace std;

/**
 * �����, ������������ ����������� ������������ ������ �����.
*/
class Descriptor {
public:
    float PI;
    Descriptor(IntegralImage *img, IPVect &ipts);
    // ��������� ���������� ������������.
    void build();
private:
    IntegralImage *img;
    IPVect &ipts;
    // ���������� ���������� i-� ������ �����.
    void findOrientation(int pointIdx);
    // ���������� ����������� i-� ������ �����.
    void buildDescriptor(int pointIdx);
    // �������� ������� ������ � �����.
    float gaussian(int x, int y, float s);
    float gaussian(float x, float y, float s);
    // ���������� ������ ����� � �����.
    float haarX(int row, int column, int s);
    float haarY(int row, int column, int s);
    // ������� ���� � �������� ��� ������� (x, y).
    float getAngle(float x, float y);
};

#endif // DESCRIPTOR_H