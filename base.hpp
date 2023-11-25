#ifndef BASE
#define BASE

#include <math.h>
#include <iostream>

using namespace std;

typedef struct{
    double x,y,z;
} R3Vector;

R3Vector addVector(R3Vector a, R3Vector b){
    R3Vector novo;
    novo.x = a.x + b.x;
    novo.y = a.y + b.y;
    novo.z = a.z + b.z;
    return novo;
}

R3Vector subVector(R3Vector a, R3Vector b){
    R3Vector novo;
    novo.x = a.x - b.x;
    novo.y = a.y - b.y;
    novo.z = a.z - b.z;
    return novo;
}

double dotProduct(R3Vector a, R3Vector b){
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

R3Vector crossProduct(R3Vector a, R3Vector b){
    R3Vector novo;
    novo.x = (a.y * b.z) - (a.z * b.y);
    novo.y = (a.z * b.x) - (a.x * b.z);
    novo.z = (a.x * b.y) - (a.y * b.x);
    return novo;
}

R3Vector scalarProduct(R3Vector a, double scalar){
    a.x *= scalar;
    a.y *= scalar;
    a.z *= scalar;
    return a;
}

double tamanho(R3Vector a){ return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);}

R3Vector normalize(R3Vector a){
    double total = tamanho(a);
    a.x = a.x/total;
    a.y = a.y/total;
    a.z = a.z/total;
    return a;
}

#endif