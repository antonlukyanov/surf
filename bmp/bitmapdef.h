#ifndef BITMAPDEF
#define BITMAPDEF

#include <iostream>

using namespace std;

#define BFT_BITMAP 0x4d42

typedef unsigned long dword;
typedef unsigned short word;
typedef unsigned char byte;

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

#endif // BITMAPDEF
