#include "bitmap.h"

Bitmap::Bitmap(string filename) {
    ifstream file(filename, ios::binary);

    if ( !file ) {
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
    if ( !bih->biSizeImage ) {
        if ( bih->biBitCount >= 8 ) {
           bih->biSizeImage = widthbytes(bih->biWidth * bih->biBitCount) * bih->biHeight;
        }
    }

    data = new char[bih->biSizeImage];
    file.read(data, bih->biSizeImage);

    file.seekg(0, ios::end);
    fileSize = file.tellg();
    file.seekg(0, ios::beg);
    file.close();

    lineLength = bih->biWidth * bytesPerPixel;
    flattenLineLength = widthbytes(lineLength * 8);
}

Bitmap::Bitmap(Bitmap &b) {
    bfh = new BitmapFileHeader;
    memcpy(bfh, b.bfh, sizeof(BitmapFileHeader));

    bih = new BitmapInfoHeader;
    memcpy(bih, b.bih, sizeof(BitmapInfoHeader));

    data = new char[bih->biSizeImage];
    memcpy(data, b.data, bih->biSizeImage);

    bytesPerPixel = b.bytesPerPixel;
    lineLength = b.lineLength;
    flattenLineLength = b.flattenLineLength;
}

Bitmap::~Bitmap() {
    if ( bfh ) {
        delete bfh;
        delete bih;
        delete[] data;
    }
}

inline dword Bitmap::widthbytes(dword n) {
    return ((n + 31) >> 5) << 2;
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
    RGBTriple p;
    for ( dword i = 0; i < bih->biHeight; ++i ) {
        for ( dword j = 0; j < bih->biWidth; ++j ) {
            at(i, j, p);
            pixelToGrayscale(p);
            setPixel(i, j, p);
        }
    }
}

void Bitmap::pixelToGrayscale(RGBTriple &p) {
    dword tmp = p.r * 0.2126 + p.g * 0.7152 + p.b * 0.0722;
    p.r = p.g = p.b = static_cast<word>(tmp);
}

bool Bitmap::getPos(dword i, dword j, dword &pos) {
    // Изображение записано снизу-вверх.
    if ( bih->biHeight > 0 ) {
        i = bih->biHeight - 1 - i;
    }

    pos = i * flattenLineLength + j * bytesPerPixel;
    if ( pos < lineLength || (i % flattenLineLength) < lineLength ) {
        return true;
    }

    return false;
}

bool Bitmap::getPos(dword i, dword &pos) {
    int padding = (i / bih->biWidth) * (bih->biWidth % 4);

    // Если изображение записано снизу-вверх.
    if ( bih->biHeight > 0 ) {
        pos = bih->biSizeImage - 1 - i * bytesPerPixel - padding;
    }
    else {
        pos = i * bytesPerPixel + padding;
    }

    if ( pos > 0 && pos < bih->biSizeImage ) {
        return true;
    }

    return false;
}

bool Bitmap::at(dword i, dword j, RGBTriple &p) {
    dword pos;
    if ( getPos(i, j, pos) ) {
        p.b = static_cast<unsigned char>(data[pos]);
        p.g = static_cast<unsigned char>(data[pos + 1]);
        p.r = static_cast<unsigned char>(data[pos + 2]);
        return true;
    }

    return false;
}

bool Bitmap::at(dword pos, RGBTriple &p) {
    if ( pos < bih->biSizeImage ) {
        p.b = static_cast<unsigned char>(data[pos]);
        p.g = static_cast<unsigned char>(data[pos + 1]);
        p.r = static_cast<unsigned char>(data[pos + 2]);
        return true;
    }

    return false;
}

void Bitmap::setPixel(dword i, dword j, RGBTriple p) {
    dword pos;
    if ( getPos(i, j, pos) ) {
        data[pos] = p.b;
        data[pos + 1] = p.g;
        data[pos + 2] = p.r;
    }
}

void Bitmap::setPixel(dword pos, RGBTriple p) {
    data[pos] = p.b;
    data[pos + 1] = p.g;
    data[pos + 2] = p.r;
}

void Bitmap::drawLine(long x0, long y0, long x1, long y1, RGBTriple p) {
    long dx = abs(x1 - x0);
    long dy = abs(y1 - y0);

    short signX = x0 < x1 ? 1 : -1;
    short signY = y0 < y1 ? 1 : -1;

    int error = dx - dy;

    setPixel(x1, y1, p);

    while ( x0 != x1 || y0 != y1 ) {
        setPixel(x0, y0, p);
        const int error2 = error * 2;

        if ( error2 > -dy ) {
            error -= dy;
            x0 += signX;
        }

        if ( error2 < dx ) {
            error += dx;
            y0 += signY;
        }
    }
}

void Bitmap::drawCirclePoints(long x0, long y0, long x, long y, RGBTriple p) {
    setPixel(x0 + x, y0 + y, p);
    setPixel(x0 + x, y0 - y, p);
    setPixel(x0 - x, y0 + y, p);
    setPixel(x0 - x, y0 - y, p);
}

void Bitmap::drawCircle(long x0, long y0, long r, RGBTriple p) {
    int x = 0;
    int y = r;
    int delta = 3 - 2 * r;
    while ( x < y ) {
        drawCirclePoints(x0, y0, x, y, p);
        drawCirclePoints(x0, y0, y, x, p);

        if ( delta < 0 ) {
            delta += 4 * x + 6;
        }
        else {
            delta += 4 * (x - y) + 10;
            y--;
        }

        x++;
    }

    if ( x == y ) {
        drawCirclePoints(x0, y0, x, y, p);
    }
}

bool Bitmap::saveAs(string filename) {
    ofstream output(filename, ios::binary);
    if ( output ) {
        output.write(reinterpret_cast<char *>(bfh), sizeof(BitmapFileHeader));
        output.write(reinterpret_cast<char *>(bih), sizeof(BitmapInfoHeader));
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
