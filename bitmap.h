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
    // Тип файла, должно быть BM (0x4d42).
    word  bfType;
    // Размер файла в байтах.
    dword bfSize;
    // Следующие два 0.
    word  bfReserved1;
    word  bfReserved2;
    // Смещение в байтах от начала структуры BitmapFileHeader до битов изображения.
    dword bfOffBits;
} BitmapFileHeader, *PBitmapFileHeader;

typedef struct tagBitmapInfoHeader {
    // Размер данной структуры в байтах.
    dword biSize;
    // Ширина изображения в пикселях.
    long  biWidth;
    // Высота изображения в пикселях. Если содержит положительное
    // значение — изображение записано в порядке снизу-вверх
    // (нулевой пиксель в нижнем левом углу). Если значение
    // отрицательное — изображение записано сверху-вниз
    // (нулевой пиксель в верхнем левом углу изображения).
    long  biHeight;
    // Количество цветовых плоскостей (в формате BMP содержит единицу).
    word  biPlanes;
    // Количество бит на пиксель.
    word  biBitCount;
    // Тип сжатия для сжатых изображений.
    dword biCompression;
    // Размер изображения в байтах. Может содержать ноль для несжатых изображений.
    dword biSizeImage;
    // Горизонтальное разрешение в пикселях на метр для целевого устройства.
    long  biXPelsPerMeter;
    // Вертикальное разрешение в пикселях на метр для целевого устройства.
    long  biYPelsPerMeter;
    // Количество используемых цветовых индексов в палитре.
    dword biClrUsed;
    // Количество элементов палитры, необходимых для отображения изображения.
    // Если содержит ноль — все индексы одинаково важны.
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
 * Класс для чтения BMP, пока только 24 бит (32 тоже должно работать).
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
    // Ширина строки изображения в байтах.
    int lineLength;
    // Выровненная ширина строки изображения в байтах.
    int flattenLineLength;

    inline dword widthbytes(dword n);
};

#endif // BITMAP_H
