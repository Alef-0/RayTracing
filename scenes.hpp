#ifndef SCENES
#define SCENES
    
#include "base.hpp"
#include <iostream>
#include <string>
#include <format>
#include <vector>
#include <array>

using namespace std;

#define SMALL 1.0e-9


class Sphere{
    R3Vector centro;
    double raio;
    R3Vector color;

    public:
    Sphere (R3Vector centro, double raio, R3Vector color){
        this->centro = centro;
        this->raio = raio;
        this->color = color;
    };

    pair<bool, double> intersect (R3Vector origem, R3Vector direcao){
        R3Vector line = subVector(origem, centro);;
        double a = dotProduct(direcao, direcao);
        double b = dotProduct(scalarProduct(line, 2), direcao);
        double c = dotProduct(line, line) - (raio * raio);

        // Praticamente bascara
        double delta = b*b - 4*a*c;
        double meio = -b/(2*a);
        if (delta < 0) {        return make_pair(false, -1);}
        else if (delta == 0) {  return make_pair(true, meio);}
        else{
            double diferenca = sqrt(delta)/(2*a);
            double t1 = meio - diferenca;
            double t2 = meio + diferenca;
            
            // Checar qual o menor
            double t;
            if (t1 < t2 && t1 >= 0) t = t1;
            else if (t2 >= 0) t = t2;
            else return make_pair(false, -1);
            return make_pair(true, t);
        }
    }

    R3Vector get_color(){
        return color;
    }
};

class Plane{
    R3Vector ponto;
    R3Vector normal;
    R3Vector color;

    public:
    Plane(R3Vector ponto,R3Vector normal,R3Vector color){
        this->ponto = ponto;
        this->normal = normal;
        this->color = color;
    }

    pair<bool, double> intersect(R3Vector origem, R3Vector direcao){
        double teste_paralelo = dotProduct(normal, direcao);
        if (fabs(teste_paralelo) < SMALL) { 
            return make_pair(false, -1);
        }
        double t = dotProduct(subVector(ponto, origem), normal)/teste_paralelo;
        return make_pair(true, t);
    }

    R3Vector get_color(){return color;}
};


class triangle{
    array<R3Vector, 3> points;
    R3Vector normal;
    R3Vector color;
    
    // Values to cache
    R3Vector v0, v1;
    double d00, d01, d11;
    double invDenom;
    
    // Razão da interseção pode ser referenciada no capítulo 3.4 de Real-Time Collision Detection
    public:
    triangle(R3Vector p1, R3Vector p2, R3Vector p3, R3Vector color){
        this->points[0] = p1;
        this->points[1] = p2;
        this->points[2] = p3;
        // Calculate normal and area
        R3Vector v0v1 = subVector(p2,p1);
        R3Vector v0v2 = subVector(p3,p1);
        this->normal = normalize(crossProduct(v0v1, v0v2));
        this->color = color;
        // Precache 
        this->v0 = subVector(points[1], points[0]); 
        this->v1 = subVector(points[2], points[0]); 
        this->d00 = dotProduct(v0, v0);
        this->d01 = dotProduct(v0, v1);
        this->d11 = dotProduct(v1, v1);
        this->invDenom = 1.0 / (d00 * d11 - d01 * d01);
    }
    pair<bool, double> intersect(R3Vector origem, R3Vector direcao){
        // Testar se pelo menos está no plano
        double teste_paralelo = dotProduct(normal, direcao);
        if (fabs(teste_paralelo) < SMALL) { 
            return make_pair(false, -1);
        }
        double t = dotProduct(subVector(points[0], origem), normal)/teste_paralelo;
        if (t < 0) return make_pair(false, -1);
        R3Vector alvo = addVector(origem, scalarProduct(direcao, t));
        
        // Ve se esta dentro
        double alpha, beta, gamma;
        tie (alpha, beta, gamma) = barycentric_cordinates(alvo);
        // cout << "Valores de alpha: " << alpha << ", beta: " << beta << ", e gamma: " << gamma << endl;
        if (alpha > 0 && beta > 0 && gamma > 0 && alpha + beta + gamma <= 1) return make_pair(true, t);
        return make_pair(false, -1);
    }
    tuple<double, double, double> barycentric_cordinates(R3Vector target){
        // Pegar os vetores para as areas
        R3Vector v2 = subVector(target, points[0]);
        // Fazer as matrizes de area
        double d20 = dotProduct(v2, v0);
        double d21 = dotProduct(v2, v1);
        // Crammer
        double beta  = (d11 * d20 - d01 * d21) * invDenom;
        double gamma = (d00 * d21 - d01 * d20) * invDenom;
        double alpha = 1.0 - beta - gamma;
        return make_tuple(alpha, beta, gamma);
    }
    R3Vector get_color() {return color;}
    bool contem(R3Vector alvo){
        for (R3Vector p : points){
            if (p.x == alvo.x && p.y == alvo.y && p.z == alvo.z) return true;
        }
        return false;
    }
};

class Mesh{
    vector <triangle> triangulos;
    vector <R3Vector> vertices;
    vector <R3Vector> normal_vertices;
    vector <array<int,3>> indices;

    public:
    Mesh(vector<triangle> tri, vector<R3Vector> vert, vector <array<int,3>> ind){
        this->triangulos = tri;
        this->vertices = vert;
        this->indices = ind;
        normal_vertices.resize(vert.size());
    }

    vector<triangle> get_triangles(){return triangulos;}
};

// int main(){
//     triangle teste = triangle(R3Vector{3,4,5},R3Vector{9,3,2},R3Vector{1,5,6}, R3Vector{1,0,0});
//     R3Vector origem = {0,0,0};
//     R3Vector direcao = {5.6,3.7,3.7};

//     bool vai; double t;
//     tie (vai, t) = teste.intersect(origem, direcao);
//     R3Vector ponto = addVector(origem, scalarProduct(direcao, t));
//     if (vai) {
//         cout << "Foi em t = " << t;         
//         printf("; Achou o ponto (%lf, %lf, %lf) \n", ponto.x, ponto.y, ponto.z);
//     }
// }

#endif