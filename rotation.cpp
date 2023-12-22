#ifndef ROTATION
#define ROTATION

#include "base.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <climits>
#include <tuple>
#include <array>

using namespace std;

R3Vector translation(R3Vector ponto, double dx, double dy, double dz){
    ponto.x += dx;
    ponto.y += dy;
    ponto.z += dz;
    return ponto;
}





#endif