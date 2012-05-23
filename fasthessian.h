#ifndef FASTHESSIAN_H
#define FASTHESSIAN_H

#include "integralimage.h"
#include "interestpointtypes.h"
#include "doh.h"
#include <map>

#define FASTHESSIAN_DBG

using namespace std;

/**
 * Класс, занимающийся подсчетом гессиана.
*/
class FastHessian {
public:
    FastHessian(
        IPVect &ipts,
        IntegralImage *img,
        int octaves = 5,
        int intervals = 4,
        int init_sampling_interval = 2,
        float threshold = 0.0004f
    );
    ~FastHessian();
    void setParameters(
        int octaves,
        int intervals,
        int init_sampling_interval,
        float threshold
    );
    void getInterestPoints();
private:
    float threshold;
    int octaves;
    int intervals;
    int step;

    IntegralImage *img;

    IPVect &ipts;

    vector<DoH *> dohVect;

    void computeDoH(int width, int height, int step, int filtersize);
    bool isMax(int row, int column, DoH *top, DoH *middle, DoH *bottom);
    void findCoordinates(int row, int column, DoH *top, DoH *middle, DoH *bottom);
};

#endif // FASTHESSIAN_H
