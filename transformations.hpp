#ifndef ROTATION
#define ROTATION

#include "base.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <climits>
#include <tuple>
#include <array>
#include <math.h>

using namespace std;
double to_radian(double x){return (x / 180.0) * M_PI; }

R3Vector translation(R3Vector ponto, double dx, double dy, double dz){
    ponto.x += dx;
    ponto.y += dy;
    ponto.z += dz;
    return ponto;
}

R3Vector rotation_x(R3Vector ponto, double angle){
    // Ponto.x permanece
    ponto.y = ponto.y * cos(angle) + ponto.z * (-sin(angle));
    ponto.z = ponto.y * sin(angle) + ponto.z * cos(angle);
    return ponto;
}

R3Vector rotation_y(R3Vector ponto, double angle){
    ponto.x = ponto.x * cos(angle) + ponto.z * sin(angle);
    // ponto y permanece
    ponto.z = ponto.x * (-sin(angle)) + ponto.z * cos(angle);
    return ponto;
}

R3Vector rotation_z(R3Vector ponto, double angle){
    ponto.x = ponto.x * cos(angle) + ponto.y * (-sin(angle));
    ponto.y = ponto.x * sin(angle) + ponto.y * cos(angle);
    // ponto z permanece
    return ponto;
}

R3Vector rotation_all(R3Vector ponto, double angle, char choice){
    angle = to_radian(angle);
    switch (choice){
        case 'x':
            return rotation_x(ponto, angle);
            break;
        case 'y':
            return rotation_y(ponto, angle);
            break;
        case 'z':
            return rotation_z(ponto, angle);
            break;
    }
}

#endif