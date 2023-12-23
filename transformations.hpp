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
const double mult_radian = M_PI / 180.0;
double to_radian(double x){return x * mult_radian; }

R3Vector translation(R3Vector ponto, double dx, double dy, double dz){
    ponto.x += dx;
    ponto.y += dy;
    ponto.z += dz;
    return ponto;
}

R3Vector rotation_x(R3Vector ponto, double angle){
    R3Vector novo;
    novo.x = ponto.x; // Ponto.x permanece
    novo.y = ponto.y * cos(angle) + ponto.z * (-sin(angle));
    novo.z = ponto.y * sin(angle) + ponto.z * cos(angle);
    return novo;
}

R3Vector rotation_y(R3Vector ponto, double angle){
    R3Vector novo;
    novo.x = ponto.x * cos(angle) + ponto.z * sin(angle);
    novo.y = ponto.y; // ponto y permanece
    novo.z = ponto.x * (-sin(angle)) + ponto.z * cos(angle);
    return novo;
}

R3Vector rotation_z(R3Vector ponto, double angle){
    R3Vector novo;
    novo.x = ponto.x * cos(angle) + ponto.y * (-sin(angle));
    novo.y = ponto.x * sin(angle) + ponto.y * cos(angle);
    novo.z = ponto.z; // ponto z permanece
    return novo;
}

R3Vector rotation_all(R3Vector ponto, double angle, char choice){
    angle = to_radian(angle);
    R3Vector novo;
    switch (choice){
        case 'x': novo = rotation_x(ponto, angle); break;
        case 'y': novo = rotation_y(ponto, angle); break;
        case 'z': novo = rotation_z(ponto, angle); break;
    }
    return novo;
}

#endif