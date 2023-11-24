#ifndef __PLANE_H__
#define __PLANE_H__

#include "vector.hpp"

class Plane{
public:
    Vector ponto;
    Vector normal;
    Vector color;

    Plane(Vector ponto,Vector normal,Vector color);

    std::pair<bool, double> intersect(Vector origem, Vector direcao);

    Vector get_color();
};

#endif