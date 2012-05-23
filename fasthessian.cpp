#include "fasthessian.h"

FastHessian::FastHessian(
        IPVect &ipts,
        IntegralImage *img,
        int octaves,
        int intervals,
        int step,
        float threshold) : ipts(ipts) {
    this->octaves = octaves;
    this->intervals = intervals;
    this->step = step;
    this->threshold = threshold;
    this->img = img;
}

FastHessian::~FastHessian() {
}

void FastHessian::setParameters(
        int octaves,
        int intervals,
        int step,
        float threshold) {
    this->octaves = octaves;
    this->intervals = intervals;
    this->step = step;
    this->threshold = threshold;
}

void FastHessian::getInterestPoints() {
    map<int, int> usedfilters;
    vector<int> filtermap(octaves * intervals);

    for ( int i = 0; i < dohVect.size(); ++i ) {
        delete dohVect[i];
    }

    dohVect.clear();

    // Обходим каждую октаву и каждый интервал.
    for ( int o = 0, index = 0, on = 1; o < octaves; o++, on++ ) {
        for ( int i = 0, in = 1; i < intervals; i++, in++ ) {
            // Размер фильтра для каждой октавы и интервала (fsize = 3 * ((2^octave) * interval + 1)).
            int fsize = 3 * (pow(static_cast<double>(2), on) * in + 1);
            if ( !usedfilters.count(fsize) ) {
                usedfilters[fsize] = index++;
                computeDoH(img->width / (step * on), img->height / (step * on), step * on, fsize);
            }
            // По сути матрица, где (i,j) элемент соответствует размеру
            // фильтра для j-го интервала i-й октавы.
            filtermap[o * intervals + i] = usedfilters[fsize];
        }
    }

#ifdef FASTHESSIAN_DBG
    cout << "---" << endl << "Filter sizes:" << endl;
    map<int, int>::iterator it;
    int i = 0;
    for ( it = usedfilters.begin(); it != usedfilters.end(); it++ ) {
        cout.width(4);
        cout << std::left << i++ << ": " << (*it).first << endl;
    }

    cout << "---" << endl << "Filter map:" << endl;
    for ( int i = 0; i < octaves * intervals; i++ ) {
        cout.width(4);
        cout << std::left << filtermap[i] << " ";
        if (i && (i + 1) % intervals == 0) {
            cout << endl;
        }
    }
#endif

    DoH *top, *middle, *bottom;

    for ( int o = 0; o < octaves; o++ ) {
        for ( int i = 0; i < 2; i++ ) {
            bottom = dohVect.at(filtermap[o * intervals + i]);
            middle = dohVect.at(filtermap[o * intervals + i + 1]);
            top = dohVect.at(filtermap[o * intervals + i + 2]);

            for ( int r = 0; r < top->height; r++ ) {
                for ( int c = 0; c < top->width; c++ ) {
                    if (isMax(r, c, top, middle, bottom)) {
                        findCoordinates(r, c, top, middle, bottom);
                    }
                }
            }
        }
    }
}

void FastHessian::computeDoH(int width, int height, int step, int filtersize) {
    int center = (filtersize - 1) / 2 + 1;
    int lobe = filtersize / 3;

    float filterarea = filtersize * filtersize;
    float Dxx, Dyy, Dxy;

    DoH *doh = new DoH(width, height, step, filtersize);

    for ( int i = 0; i < height; i++ ) {
        for ( int j = 0; j < width; j++ ) {
            int row = i * step;
            int column = j * step;

            Dxx = (img->sum(row - lobe + 1, column - center, 2 * lobe - 1, filtersize)
                - 3 * img->sum(row - lobe + 1, column - lobe / 2, 2 * lobe - 1, lobe)) / filterarea;

            Dyy = (img->sum(row - center, column - lobe + 1, filtersize, 2 * lobe - 1)
                - 3 * img->sum(row - lobe / 2, column - lobe + 1, lobe, 2 * lobe - 1)) / filterarea;

            Dxy = (img->sum(row - lobe, column + 1, lobe, lobe)
                + img->sum(row + 1, column - lobe, lobe, lobe)
                - img->sum(row - lobe, column - lobe, lobe, lobe)
                - img->sum(row + 1, column + 1, lobe, lobe)) / filterarea;

            float hessian = (Dxx * Dyy - 0.912f * Dxy * Dxy);
            doh->determinant[i * width + j] = hessian < 0 ? 0 : hessian;
            doh->laplacian[i * width + j] = Dxx + Dyy < 0 ? 0 : 1;
        }
    }

    dohVect.push_back(doh);
}

bool FastHessian::isMax(int row, int column, DoH *top, DoH *middle, DoH *bottom) {
    // Не учитываем точки, значения которых были получены с помощью
    // фильтров, которые выходили за границы изображения.
    int layerBorder = (top->filtersize + 1) / (2 * top->step);
    if (row <= layerBorder || row >= top->height - layerBorder || column <= layerBorder || column >= top->width - layerBorder) {
        return false;
    }

    float det = middle->getDet(row, column, top);
    if (det < threshold ) {
        return false;
    }

    for ( int i = -1; i <= 1; i++ ) {
        for ( int j = -1; j <= 1; j++ ) {
            if ( top->getDet(row + i, column + j) >= det ||
                ((i != 0 || j != 0) && middle->getDet(row + i, column + j, top) >= det) ||
                bottom->getDet(row + i, column + j, top) >= det ) {
                return false;
            }
        }
    }

    return true;
}

void FastHessian::findCoordinates(int row, int column, DoH *top, DoH *middle, DoH *bottom) {
    double dx, dy, ds;

    // dx соответствует производной dI/dx, остальные аналогично.
    dx = (middle->getDet(row, column + 1, top) - middle->getDet(row, column - 1, top)) / 2.0;
    dy = (middle->getDet(row + 1, column, top) - middle->getDet(row - 1, column, top)) / 2.0;
    ds = (top->getDet(row, column) - bottom->getDet(row, column, top)) / 2.0;

    // dH/dx.
    double dH[3] = {
        dx,
        dy,
        ds
    };

    // dxx соответствует производной d^2I/dx^2, dxy соответствует d^2I/dxdy и т.д.
    double dxx, dxy, dxs, dyy, dys, dss, t;

    t = middle->getDet(row, column, top);
    dxx = middle->getDet(row, column + 1, top) + middle->getDet(row, column - 1, top) - 2 * t;
    dyy = middle->getDet(row + 1, column, top) + middle->getDet(row - 1, column, top) - 2 * t;
    dss = top->getDet(row, column) + bottom->getDet(row, column, top) - 2 * t;
    dxy = (middle->getDet(row + 1, column + 1, top) - middle->getDet(row + 1, column - 1, top) - 
           middle->getDet(row - 1, column + 1, top) + middle->getDet(row - 1, column - 1, top)) / 4.0;
    dxs = (top->getDet(row, column + 1) - top->getDet(row, column - 1) - 
           bottom->getDet(row, column + 1, top) + bottom->getDet(row, column - 1, top)) / 4.0;
    dys = (top->getDet(row + 1, column) - top->getDet(row - 1, column) - 
           bottom->getDet(row + 1, column, top) + bottom->getDet(row - 1, column, top)) / 4.0;

    // Определитель матрицы ((d^2H/dx^2):
    // dxx dxy dxs
    // dxy dyy dys
    // dxs dys dss
    double detH = dxx * (dyy * dss - dys * dys) - dxy * (dxy * dss - dxs * dys) + dxs * (dxy * dys - dxs * dyy);
    if (detH != 0) {
        // Обратная матрица для вышеприведенной ((d^2H/dx^2)^-1).
        double d2H_inv[3][3] = {
            {(dyy * dss - dys * dys) / detH, (dxs * dys - dxy * dss) / detH, (dxy * dys - dxs * dyy) / detH},
            {(dys * dxs - dxy * dss) / detH, (dxx * dss - dxs * dxs) / detH, (dxs * dxy - dxx * dys) / detH},
            {(dxy * dys - dyy * dxs) / detH, (dxy * dxs - dxx * dys) / detH, (dxx * dyy - dxy * dxy) / detH}
        };

        double X[3];
        // X = (d^2H/dx^2)^-1 * dH/dx. Результирующий вектор со значениями сдвигов.
        for ( int i = 0; i < 3; i++ ) {
            double sum = 0;
            for ( int j = 0; j < 3; j++ ) {
                sum += d2H_inv[i][j] * dH[j];
            }
            X[i] = -sum;
        }

        double row_offset = X[1], column_offset = X[0], scale_offset = X[2];
        // Если значения сдвигов больше 0.5, то экстремум лежит ближе к другой точке, иначе уточняем координаты.
        if ( fabs(scale_offset) < 0.5f  &&  fabs(row_offset) < 0.5f  &&  fabs(column_offset) < 0.5f ) {
            int filterStep = middle->filtersize - bottom->filtersize;
            InterestPoint p;
            // Восстанавливаем координаты точек и масштаб с учетом сдвигов.
            p.x = static_cast<float>((column + column_offset) * top->step);
            p.y = static_cast<float>((row + row_offset) * top->step);
            p.scale = static_cast<float>((0.1333f) * (middle->filtersize + scale_offset * filterStep));
            p.laplacian = static_cast<int>(middle->getLaplacian(row, column, top));
            ipts.push_back(p);
        }
    }
}