#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "vector.hpp"

class Sphere{
public:
    Vector centro;
    double raio;
    Vector color;

    Sphere(Vector centro, double raio, Vector color);

    std::pair<bool, double> intersect (Vector origem, Vector direcao);

    Vector get_color();
};

#endif