#include "descriptor.h"

Descriptor::Descriptor(IntegralImage *img, IPVect &ipts) : ipts(ipts) {
    this->img = img;
    this->PI = 3.14159;
}

void Descriptor::build() {
    // Находим ориентацию особых точек.
    for ( int i = 0; i < ipts.size(); i++ ) {
        findOrientation(i);
        buildDescriptor(i);
    }
}

void Descriptor::findOrientation(int pointIdx) {
    InterestPoint *p = &ipts.at(pointIdx);

    int scale = fround(p->scale);
    int r = fround(p->y);
    int c = fround(p->x);

    float haarResX[109];
    float haarResY[109];

    for ( int i = -6, index = 0; i <= 6; i++ ) {
        for ( int j = -6; j <= 6; j++ ) {
            if ( i*i + j*j < 36 ) {
                float gauss = gaussian(i, j, 2.0);
                haarResX[index] = gauss * haarX(r + j*scale, c + i*scale, 4*scale);
                haarResY[index] = gauss * haarY(r + j*scale, c + i*scale, 4*scale);
                index++;
            }
        }
    }

    // Координаты суммарного вектора.
    float X;
    float Y;
    // Максимальная длина суммарного вектора.
    float maxlen = 0;
    // Угол (ориентация) максимального суммарного вектора.
    float orientation = 0;
    // 0.15 ( = 1/2*pi) здесь примерно 1 градус в радианах.
    for ( float ang1 = 0; ang1 < 2*PI; ang1 += 0.15 ) {
        // Если ang1 + pi/3 > 2pi, то ang2 = pi/3 - alpha, где alpha = 2pi - ang1.
        float ang2 = ang1 + PI/3.0 > 2*PI ? ang1 - 5.0*PI/3.0 : ang1 + PI/3.0;
        X = 0;
        Y = 0;
        for ( int i = 0; i < 109; i++ ) {
            float ang = getAngle(haarResX[i], haarResY[i]);
            if ( ang1 < ang2 && ang1 < ang && ang < ang2
                 // Через окно проходит y = 0.
                 || ang1 > ang2 && (ang < ang2 || (ang1 < ang && ang < 2*PI))) {
                X += haarResX[i];  
                Y += haarResY[i];
            } 
        }

        float len = X*X + Y*Y;
        // За ориентацию особой точки принимаем угол суммарного вектора с максимальной длиной.
        if (len > maxlen) {
            maxlen = len;
            orientation = getAngle(X, Y);
        }
    }

    p->orientation = orientation;
}

void Descriptor::buildDescriptor(int pointIdx) {
    InterestPoint *p = &ipts.at(pointIdx);

    float psin = sin(p->orientation);
    float pcos = cos(p->orientation);

    float scale = p->scale;
    int x = fround(p->y);
    int y = fround(p->x);

    float norm = 0;
    for ( int i = 0, index = 0; i < 4; i++ ) {
        for ( int j = 0; j < 4; j++ ) {
            float dx = 0, dy = 0;
            float mdx = 0, mdy = 0;
            for ( int k = 0; k < 5; k++ ) {
                for ( int l = 0; l < 5; l++ ) {
                    int u = fround(x + (-psin * ((j-2)*5+l) * scale + pcos * ((i-2)*5+k) * scale));
                    int v = fround(y + ( pcos * ((j-2)*5+l) * scale + psin * ((i-2)*5+k) * scale));
                    float responseX = haarX(v, u, 2 * fround(scale));
                    float responseY = haarY(v, u, 2 * fround(scale));
                    float gauss = gaussian((i-2)*5 + k, (j-2)*5 + l, 2.5*scale);
                    float responseU = gauss * (-responseX * psin + responseY * pcos);
                    float responseV = gauss * ( responseX * pcos + responseY * psin);
                    dx += responseU;
                    dy += responseV;
                    mdx += fabs(responseU);
                    mdy += fabs(responseV);
                }
            }
            p->descriptor[index++] = dx;
            p->descriptor[index++] = dy;
            p->descriptor[index++] = mdx;
            p->descriptor[index++] = mdy;

            norm += dx*dx + dy*dy + mdx*mdx + mdy*mdy;
        }
    }

    norm = sqrt(norm);
    for ( int i = 0; i < 64; i++ ) {
        p->descriptor[i] /= norm;
    }
}

float Descriptor::gaussian(int x, int y, float s) {
  return (1.0 / (2.0*PI*s*s)) * exp( -(x*x + y*y) / (2.0*s*s) );
}

float Descriptor::gaussian(float x, float y, float s) {
  return (1.0 / (2.0*PI*s*s)) * exp( -(x*x + y*y) / (2.0*s*s) );
}

float Descriptor::haarX(int row, int column, int s) {
  return img->sum(row - s/2, column, s, s/2)
      -1 * img->sum(row - s/2, column - s/2, s, s/2);
}

float Descriptor::haarY(int row, int column, int s) {
  return img->sum(row, column - s/2, s/2, s)
      -1 * img->sum(row - s/2, column - s/2, s/2, s);
}

float Descriptor::getAngle(float x, float y) {
    if ( x < 0 && y < 0 || x > 0 && y < 0 ) {
        return 2*PI + atan2(y, x);
    }
    else {
        return atan2(y, x);
    }

    return 0;
}
