#include <iostream>
#include "bitmap.h"
#include "integralimage.h"
#include "interestpointtypes.h"
#include "fasthessian.h"
#include "descriptor.h"
#include "misc.h"

using namespace std;

int main(int argc, char **argv) {
    Bitmap b("IMG_2436.bmp");
    Bitmap newb(b);

    b.toGrayscale();
    IntegralImage *img = new IntegralImage(b);

    IPVect ipts;

    FastHessian fh(ipts, img, 5, 4, 2, 0.004f);
    fh.getInterestPoints();

    Descriptor descr(img, ipts);
    descr.build();

    for ( int i = 0; i < ipts.size(); i++ ) {
        InterestPoint *ipt = &ipts.at(i);

        int r1 = fround(ipt->y);
        int c1 = fround(ipt->x);
        float o = ipt->orientation;
        float s = 2.5f * ipt->scale;

        int c2 = fround(s * cos(o)) + c1;
        int r2 = fround(s * sin(o)) + r1;

        if ( o ) {
            newb.drawLine(r1, c1, r2, c2, RGB(0, 0, 0));
        }

        newb.drawCircle(r1, c1, s, RGB(0, 0, 0));
    }

    newb.saveAs("1.bmp");

    cout << "---" << endl << ipts.size() << " points" << endl;

    delete img;

    return 0;
}