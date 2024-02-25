#ifndef BASE
#define BASE

#include <math.h>
#include <iostream>
#include <array>

using namespace std;

struct R3Vector{
    double x,y,z;
} ;
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
R3Vector simpleProduct(R3Vector a, R3Vector b){
    R3Vector novo;
    novo.x = a.x * b.x;
    novo.y = a.y * b.y;
    novo.z = a.z * b.z;
    return novo;
}
double tamanho(R3Vector a){ return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);}
R3Vector normalize(R3Vector a){
    double total = tamanho(a);
    a.x = a.x / total;
    a.y = a.y / total;
    a.z = a.z / total;
    return a;
}

R3Vector refract(R3Vector normal, R3Vector vector, double IOR, int depth){ // Todos os valores precisam estar normalizados e saindo da superficie
    double n;
    double cos1;

    // Checagem do nivel de profundidade pra determinar se o raio está indo do ar -> objeto ou objeto -> ar
    if(depth%2 != 0){
        cos1 = dotProduct(normal, vector);
        n = IOR;  
    } 
    else{
        cos1 = dotProduct(normal, scalarProduct(vector, -1));
        n = 1/IOR;
    } 

    double sin1 = 1 - pow(cos1, 2);
    double sin2 = sin1*n;
    double cos2 = 1 - pow(sin2, 2);
    
    
    R3Vector t = addVector(scalarProduct(vector, n), scalarProduct(normal, (n*cos1 - cos2)));

    return normalize(t);
}

R3Vector reflect(R3Vector normal, R3Vector vector){ // Todos os valores precisam estar normalizados e saindo da superficie
    double value = dotProduct(normal, vector) * 2.0;
    R3Vector left = scalarProduct(normal, value);
    return normalize(subVector(left, vector));
}
double distancia(R3Vector a, R3Vector b){
    return tamanho(subVector(a, b));
}


#endif