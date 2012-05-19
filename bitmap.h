#ifndef BITMAP_H
#define BITMAP_H

#include <iostream>
#include <string>
#include <fstream>
#include <math.h>

#include "types.h"

using namespace std;

#pragma pack(2)

typedef struct tagBitmapFileHeader {
    // ��� �����, ������ ���� BM (0x4d42).
    word  bfType;
    // ������ ����� � ������.
    dword bfSize;
    // ��������� ��� 0.
    word  bfReserved1;
    word  bfReserved2;
    // �������� � ������ �� ������ ��������� BitmapFileHeader �� ����� �����������.
    dword bfOffBits;
} BitmapFileHeader, *PBitmapFileHeader;

typedef struct tagBitmapInfoHeader {
    // ������ ������ ��������� � ������.
    dword biSize;
    // ������ ����������� � ��������.
    long  biWidth;
    // ������ ����������� � ��������. ���� �������� �������������
    // �������� � ����������� �������� � ������� �����-�����
    // (������� ������� � ������ ����� ����). ���� ��������
    // ������������� � ����������� �������� ������-����
    // (������� ������� � ������� ����� ���� �����������).
    long  biHeight;
    // ���������� �������� ���������� (� ������� BMP �������� �������).
    word  biPlanes;
    // ���������� ��� �� �������.
    word  biBitCount;
    // ��� ������ ��� ������ �����������.
    dword biCompression;
    // ������ ����������� � ������. ����� ��������� ���� ��� �������� �����������.
    dword biSizeImage;
    // �������������� ���������� � �������� �� ���� ��� �������� ����������.
    long  biXPelsPerMeter;
    // ������������ ���������� � �������� �� ���� ��� �������� ����������.
    long  biYPelsPerMeter;
    // ���������� ������������ �������� �������� � �������.
    dword biClrUsed;
    // ���������� ��������� �������, ����������� ��� ����������� �����������.
    // ���� �������� ���� � ��� ������� ��������� �����.
    dword biClrImportant;
} BitmapInfoHeader, *PBitmapInfoHeader;

typedef struct tagRGBTriple {
    word r, g, b;
} RGBTriple, *PRGBTriple;

#pragma pack()

inline RGBTriple RGB(word r, word g, word b) {
    RGBTriple p;
    p.r = r;
    p.g = g;
    p.b = b;

    return p;
}

/**
 * ����� ��� ������ BMP, ���� ������ 24 ��� (32 ���� ������ ��������).
*/
class Bitmap {
public:
    Bitmap(string filename);
    Bitmap(Bitmap &b);
    ~Bitmap();

    bool save();
    bool saveAs(string filename);

    long getWidth();
    long getHeight();
    word getBitCount();
    dword getFileSize();
    word getPlanes();

    void toGrayscale();
    void pixelToGrayscale(RGBTriple &p);

    bool at(dword i, dword j, RGBTriple &p);
    bool at(dword pos, RGBTriple &p);

    void drawLine(long x0, long y0, long x1, long y1, RGBTriple p);
    void drawCirclePoints(long x0, long y0, long x, long y, RGBTriple p);
    void drawCircle(long x0, long y0, long r, RGBTriple p);

    void setPixel(dword i, dword j, RGBTriple p);
    void setPixel(dword pos, RGBTriple p);

    BitmapFileHeader *bfh;
    BitmapInfoHeader *bih;
    char *data;

    int bytesPerPixel;
    unsigned long fileSize;

    bool getPos(dword i, dword j, dword &pos);
    bool getPos(dword i, dword &pos);
private:
    // ������ ������ ����������� � ������.
    int lineLength;
    // ����������� ������ ������ ����������� � ������.
    int flattenLineLength;

    inline dword widthbytes(dword n);
};

#endif // BITMAP_H
