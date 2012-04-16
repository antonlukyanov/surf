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

#endif // BITMAPDEF
