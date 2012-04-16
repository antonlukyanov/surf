#ifndef BITMAP
#define BITMAP

#include <iostream>
#include <string>
#include <fstream>
#include <math.h>

#include "bitmapdef.h"

using namespace std;

class Bitmap {
public:
  Bitmap(string filename);
  ~Bitmap();

  bool save();
  bool saveAs(string filename);

  long getWidth();
  long getHeight();
  word getBitCount();
  dword getFileSize();
  word getPlanes();

  void toGrayscale();
  void grayscaleAt();

  void at(long i, long j, RGBTriple &p);

  void drawLine(long x0, long y0, long x1, long y1, RGBTriple p);
  void drawCircle(long x0, long y0, long r, RGBTriple p);

  void setPixel(long i, long j, RGBTriple p);
  void setPixel(long pos, RGBTriple p);

  BitmapFileHeader *bfh;
  BitmapInfoHeader *bih;
  char *data;

  int bytesPerPixel;
  unsigned long fileSize;
private:
  ifstream file;
  bool getPos(dword i, dword j, dword &pos);
};

#endif // BITMAP
