#include <iostream>
#include "bitmap.h"
#include "integralimage.h"
#include "interestpointtypes.h"
#include "fasthessian.h"

using namespace std;

int main(int argc, char **argv) {
    Bitmap b("IMG_2436.bmp");
    Bitmap newb(b);

    b.toGrayscale();
    IntegralImage *img = new IntegralImage(b);

    IPVect ipts;
    FastHessian fh(ipts, img, 5, 4, 2, 0.008f);
    fh.getPoints();

    InterestPoint *ipt;
    float s, o;
    int r1, c1;

    for ( int i = 0; i < ipts.size(); i++ ) {
        ipt = &ipts.at(i);
        s = (2.5 * ipt->scale);
        o = ipt->orientation;
        r1 = (int)floor(ipt->y + 0.5f);
        c1 = (int)floor(ipt->x + 0.5f);
        s = (int)floor(s + 0.5f);
        cout << r1 << "," << c1 << " - " << s << endl;
        newb.drawCircle(r1, c1, s, RGB(255, 255, 255));
    }

    newb.saveAs("1.bmp");

    cout << ipts.size() << endl;

    delete img;

    return 0;
}