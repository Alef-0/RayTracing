#ifndef PHONG
#define PHONG

#include "base.hpp"
#include <vector>

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

R3Vector phong_pixel(Phong colided, R3Vector origem, vector<Light> luzes, 
                        R3Vector luz_ambiente, R3Vector alvo, R3Vector normal){
    R3Vector answer;
    R3Vector Viewer = normalize(subVector(origem, alvo)); // Calcular vetores de direcao da pessoa
    answer = simpleProduct(luz_ambiente, colided.k_ambiente); // Calcular ambiente
    // Somar tudo
    for (Light luz : luzes){
        R3Vector L_direcao = normalize(subVector(luz.origem, alvo));
        if (dotProduct(normal, L_direcao) < 0) continue; // Tem certeza que vai atingir
        // Difusao
        double LN = dotProduct(L_direcao, normal);
        R3Vector difusao = simpleProduct(colided.k_difuso, luz.intensidade);
        difusao = scalarProduct(difusao, LN);
        answer = addVector(answer, difusao);
        // Especular
        R3Vector R = subVector(scalarProduct(normal, dotProduct(L_direcao, normal) * 2), L_direcao);
        R = normalize(R);
        R3Vector especular = simpleProduct(colided.k_especular, luz.intensidade);
        double RVa = pow(dotProduct(R, Viewer), colided.rugosidade);
        especular = scalarProduct(especular, RVa);
        answer = addVector(especular, answer);
    }
    return answer;
}

#endif