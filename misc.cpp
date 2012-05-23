#include "misc.h"

int fround(float n) {
    return static_cast<int>(floor(n + 0.5));
}
