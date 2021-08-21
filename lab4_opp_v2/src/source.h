#ifndef LAB4_OPP_V2_SOURCE_H
#define LAB4_OPP_V2_SOURCE_H

#include <iostream>

namespace lab4_consts {
    const long double a = 1e5;
    const long double epsilon = 1e-8;
    const long double Dx = 2.0;
    const long double Dy = 2.0;
    const long double Dz = 2.0;
    const long double X0 = -1.0;
    const long double Y0 = -1.0;
    const long double Z0 = -1.0;
    const int dimension = 32;
}

void *safeCalloc(size_t count, size_t sizeofType);
void *safeMalloc(size_t sizeofType, size_t count);

#endif //LAB4_OPP_V2_SOURCE_H
