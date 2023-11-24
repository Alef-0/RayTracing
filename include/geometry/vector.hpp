#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <math.h>

class Vector
{
public:
    Vector(double x = 0, double y = 0, double z = 0);
    ~Vector();

    double x, y, z;

    static Vector addVector(Vector a, Vector b);

    static Vector subVector(Vector a, Vector b);

    static double dotProduct(Vector a, Vector b);

    static Vector crossProduct(Vector a, Vector b);

    static Vector scalarProduct(Vector a, double scalar);

    static double tamanho(Vector a);

    static Vector normalize(Vector a);
    
};



#endif