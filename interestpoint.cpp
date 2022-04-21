#include "interestpoint.h"

float InterestPoint::operator-(const InterestPoint &p) {
    float sum = 0;
    for ( int i = 0; i < 64; ++i ) {
        sum += (this->descriptor[i] - p.descriptor[i]) * (this->descriptor[i] - p.descriptor[i]);
    }
    return sqrt(sum);
};
