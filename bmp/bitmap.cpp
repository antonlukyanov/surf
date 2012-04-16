#include "bitmap.h"

Bitmap::Bitmap(string filename) {
  file.open(filename, ios::binary);

  if (!file) {
    return;
  }

  bfh = new BitmapFileHeader;
  memset(bfh, 0, sizeof(BitmapFileHeader));
  bih = new BitmapInfoHeader;
  memset(bih, 0, sizeof(BitmapInfoHeader));

  file.read((char *)bfh, sizeof(BitmapFileHeader));
  file.read((char *)bih, sizeof(BitmapInfoHeader));

  bytesPerPixel = bih->biBitCount / 8;

  // Если отсутствует размер изображения в байтах, то находим его.
  if (!bih->biSizeImage) {
    if (bih->biBitCount >= 8) {
      bih->biSizeImage = bih->biWidth * bih->biHeight * bytesPerPixel;
    }
  }

  data = new char[bih->biSizeImage];
  file.read(data, bih->biSizeImage);

  file.seekg(0, ios::end);
  fileSize = file.tellg();
  file.seekg(0, ios::beg);

  file.close();
}

Bitmap::~Bitmap() {
  if (file) {
    delete bfh;
    delete bih;
    delete[] data;
  }
}

long Bitmap::getWidth() {
  return bih->biWidth;
}

long Bitmap::getHeight() {
  return abs(bih->biHeight);
}

dword Bitmap::getFileSize() {
  return fileSize;
}

word Bitmap::getPlanes() {
  return bih->biPlanes;
}

word Bitmap::getBitCount() {
  return bih->biBitCount;
}

void Bitmap::toGrayscale() {

}

void Bitmap::at(long i, long j, RGBTriple &p) {
  dword pos;
  if (getPos(i, j, pos)) {
    p.b = static_cast<unsigned char>(data[pos++]);
    p.g = static_cast<unsigned char>(data[pos++]);
    p.r = static_cast<unsigned char>(data[pos]);
  }
}

void Bitmap::setPixel(long i, long j, RGBTriple p) {
  dword pos;
  if (getPos(i, j, pos)) {
    data[pos++] = p.b;
    data[pos++] = p.g;
    data[pos] = p.r;
  }
}

void Bitmap::setPixel(long pos, RGBTriple p) {
  data[pos++] = p.b;
  data[pos++] = p.g;
  data[pos] = p.r;
}

void Bitmap::drawLine(long x0, long y0, long x1, long y1, RGBTriple p) {
  long dx = abs(x1 - x0);
  long dy = abs(y1 - y0);

  short signX = x0 < x1 ? 1 : -1;
  short signY = y0 < y1 ? 1 : -1;

  int error = dx - dy;

  setPixel(x1, y1, p);

  while (x0 != x1 || y0 != y1) {
    setPixel(x0, y0, p);
    const int error2 = error * 2;

    if (error2 > -dy) {
      error -= dy;
      x0 += signX;
    }

    if (error2 < dx) {
      error += dx;
      y0 += signY;
    }
  }
}

void Bitmap::drawCircle(long x0, long y0, long r, RGBTriple p) {
  int x = 0;
  int y = r;
  int delta = 2 - 2 * r;
  int error = 0;
  while (y >= 0) {
    setPixel(x0 + x, y0 + y, p);
    setPixel(x0 + x, y0 - y, p);
    setPixel(x0 - x, y0 + y, p);
    setPixel(x0 - x, y0 - y, p);
    error = 2 * (delta + y) - 1;
    if (delta < 0 && error <= 0) {
      ++x;
      delta += 2 * x + 1;
      continue;
    }

    error = 2 * (delta - x) - 1;
    if (delta > 0 && error > 0) {
      --y;
      delta += 1 - 2 * y;
      continue;
    }

    ++x;
    delta += 2 * (x - y);
    --y;
  }
}

bool Bitmap::getPos(dword i, dword j, dword &pos) {
  int lineLength = bih->biWidth * bytesPerPixel;
  int flattenLineLength = lineLength + (bih->biWidth % 4);

  if (bih->biHeight > 0) {
    i = bih->biHeight - 1 - i;
  }

  pos = i * flattenLineLength + j * bytesPerPixel;
  if (pos < lineLength || (i % flattenLineLength) < lineLength) {
    return true;
  }

  return false;
}

bool Bitmap::saveAs(string filename) {
  ofstream output(filename);
  if (output) {
    output.write((char *)bfh, sizeof(BitmapFileHeader));
    output.write((char *)bih, sizeof(BitmapInfoHeader));
    output.write(data, bih->biSizeImage);
    output.close();
  }
  else {
    return false;
  }

  return true;
}

bool Bitmap::save() {
  return false;
}
