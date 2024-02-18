#ifndef CLASSES
#define CLASSES
    
#include "base.hpp"
#include "transformations.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <tuple>

using namespace std;
#define SMALL 1.0e-9

class Phong{
    public:
    R3Vector k_ambiente;
    R3Vector k_difuso;
    R3Vector k_especular;
    double rugosidade;
};

class Light{
    public:
    R3Vector origem;
    R3Vector intensidade;

    Light (R3Vector orig, R3Vector intens){
        this->origem = orig;
        this->intensidade = intens;
    }
};

class Sphere: public Phong{
    public:
    R3Vector centro;
    double raio;

    Sphere (R3Vector centro, double raio, R3Vector ka, R3Vector kd, R3Vector ks, double rug){
        this->centro = centro;
        this->raio = raio;
        this->k_ambiente = ka;
        this->k_difuso = kd;
        this->k_especular = ks;
        this->rugosidade = rug;
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
    void auto_translation(double dx, double dy, double dz){
        this->centro = translation(centro, dx, dy, dz);
        // Não muda mais nada
    }
    void auto_rotation(double angle, char choice){
        centro = rotation_all(centro, angle, choice);
    }
};

class Plane: public Phong{
    public:
    R3Vector ponto;
    R3Vector normal;

    Plane(R3Vector ponto,R3Vector normal, R3Vector ka, R3Vector kd, R3Vector ks, double rug){
        this->ponto = ponto;
        this->normal = normalize(normal);
        this->k_ambiente = ka;
        this->k_difuso = kd;
        this->k_especular = ks;
        this->rugosidade = rug;
    }
    pair<bool, double> intersect(R3Vector origem, R3Vector direcao){
        double teste_paralelo = dotProduct(normal, direcao);
        if (fabs(teste_paralelo) < SMALL) { 
            return make_pair(false, -1);
        }
        double t = dotProduct(subVector(ponto, origem), normal)/teste_paralelo;
        return make_pair(true, t);
    }

    Plane* get_plane(){return this;}
    void auto_translation(double dx, double dy, double dz){
        this->ponto = translation(this->ponto, dx,dy, dz);
        // Normal permanece
    }
    void auto_rotation(double angle, char choice){
        ponto = rotation_all(ponto, angle, choice);
        normal = rotation_all(normal, angle, choice);
    }
};

class triangle: public Phong{
    public:
    array<R3Vector, 3> points;
    R3Vector normal;
    
    // Values to cache
    R3Vector v0, v1;
    double d00, d01, d11;
    double invDenom;
    
    // Razão da interseção pode ser referenciada no capítulo 3.4 de Real-Time Collision Detection
    triangle(R3Vector p1, R3Vector p2, R3Vector p3, R3Vector ka, R3Vector kd, R3Vector ks, double rug){
        this->points[0] = p1;
        this->points[1] = p2;
        this->points[2] = p3;
        this->k_ambiente = ka;
        this->k_difuso = kd;
        this->k_especular = ks;
        this->rugosidade = rug;

        calcular_constantes();
    }
    
    void calcular_constantes(){   
        // Calculate normal and area
        R3Vector v0v1 = subVector(this->points[1], this->points[0]);
        R3Vector v0v2 = subVector(this->points[2], this->points[0]);
        this->normal = normalize(crossProduct(v0v1, v0v2));
        // Precache constantes
        this->v0 = subVector(this->points[1], this->points[0]); 
        this->v1 = subVector(this->points[2], this->points[0]); 
        this->d00 = dotProduct(this->v0, this->v0);
        this->d01 = dotProduct(this->v0, this->v1);
        this->d11 = dotProduct(this->v1, this->v1);
        this->invDenom = 1.0 / (this->d00 * this->d11 - this->d01 * this->d01);
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
        if (alpha >= 0 && beta >= 0 && gamma >= 0) return make_pair(true, t);
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
    bool contem(R3Vector alvo){
        for (R3Vector p : points){
            if (p.x == alvo.x && p.y == alvo.y && p.z == alvo.z) return true;
        }
        return false;
    }
    void auto_translation(double dx, double dy, double dz){
        this->points[0] = translation(this->points[0], dx,dy,dz);
        this->points[1] = translation(this->points[1], dx,dy,dz);
        this->points[2] = translation(this->points[2], dx,dy,dz);
        calcular_constantes(); // melhor prevenir
    }
    void auto_rotation(double angle, char choice){
        // Retornar os valores iniciais
        this->points[0] = rotation_all(this->points[0], angle, choice); 
        this->points[1] = rotation_all(this->points[1], angle, choice); 
        this->points[2] = rotation_all(this->points[2], angle, choice); 
        // Calcular a normal
        calcular_constantes();
    }
};

class Mesh{
    public:
    vector <triangle> triangulos;
    vector <R3Vector> vertices;
    vector <R3Vector> normal_vertices;
    vector <array<int,3>> indices;

    Mesh(vector<triangle> tri, vector<R3Vector> vert, vector <array<int,3>> ind){
        this->triangulos = tri;
        this->vertices = vert;
        this->indices = ind;
        normal_vertices.resize(vert.size());
    }

    vector<triangle>* get_triangles(){return &triangulos;}
    vector<triangle> return_triangles(){return triangulos;}
};

class Everything{
    public:
    vector<Sphere> esferas;
    vector<Plane> planos;
    vector<triangle> triangulos;

    Everything(vector<Sphere> esferas,vector<Plane> planos, vector<triangle> triangulos){
        this->esferas = esferas;
        this->planos = planos;
        this->triangulos = triangulos;
    }
};

#endif