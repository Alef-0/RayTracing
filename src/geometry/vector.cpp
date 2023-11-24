#include "vector.hpp"

Vector::Vector(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector::~Vector()
{
}

Vector Vector::addVector(Vector a, Vector b){
    Vector novo;
    novo.x = a.x + b.x;
    novo.y = a.y + b.y;
    novo.z = a.z + b.z;
    return novo;
}

Vector Vector::subVector(Vector a, Vector b){
    Vector novo = Vector(0, 0, 0);
    novo.x = a.x - b.x;
    novo.y = a.y - b.y;
    novo.z = a.z - b.z;
    return novo;
}

double Vector::dotProduct(Vector a, Vector b){
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector Vector::crossProduct(Vector a, Vector b){
    Vector novo = Vector(0, 0, 0);
    novo.x = (a.y * b.z) - (a.z * b.y);
    novo.y = (a.z * b.x) - (a.x * b.z);
    novo.z = (a.x * b.y) - (a.y * b.x);
    return novo;
}

Vector Vector::scalarProduct(Vector a, double scalar){
    a.x *= scalar;
    a.y *= scalar;
    a.z *= scalar;
    return a;
}

double Vector::tamanho(Vector a){ return sqrt(a.x*a.x + a.y*a.y + a.z*a.z); }

Vector Vector::normalize(Vector a){
    double total = Vector::tamanho(a);
    a.x = a.x/total;
    a.y = a.y/total;
    a.z = a.z/total;
    return a;
}