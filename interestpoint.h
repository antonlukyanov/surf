#ifndef IPOINT_H
#define IPOINT_H

#include <iostream>

using namespace std;

/**
 * ������ �����.
*/
class InterestPoint {
public:
    // ���������� ��������� ������ �����.
    float x, y;
    // �������, � ������� ���� ������� ������ �����.
    float scale;
    // ����������� ������ �����.
    float orientation;
    // ����������, ��������� �� 64 ���������.
    float descriptor[64];
    // ���� ���������� ��� ����������� �� ����� ���� ���� ������� �����.
    int laplacian;

    float operator-(const InterestPoint &p);
};

#endif // IPOINT_H
