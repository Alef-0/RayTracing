#include "plane.hpp"

Vector ponto;
Vector normal;
Vector color;

Plane::Plane(Vector ponto,Vector normal,Vector color){
    this->ponto = ponto;
    this->normal = normal;
    this->color = color;
}

std::pair<bool, double> Plane::intersect(Vector origem, Vector direcao){
    double teste_paralelo = Vector::dotProduct(this->normal, direcao);
    if (teste_paralelo == 0) { 
        // cout << "É paralelo, e não se cruza" << endl;
        return std::make_pair(false, 0);
    }
    else{
        double t = (Vector::dotProduct(normal, ponto) - Vector::dotProduct(this->normal, origem))/teste_paralelo;
        // cout << "Ele se cruza em " << t << endl;
        // if (t < 0) cout << "Entretanto é atrás da tela" << endl;
        return std::make_pair(true, t);
    }
}

Vector Plane::get_color(){
    return this->color;
}