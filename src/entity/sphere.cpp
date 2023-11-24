#include "sphere.hpp"

Sphere::Sphere(Vector centro, double raio, Vector color){
    this->centro = centro;
    this->raio = raio;
    this->color = color;
};

std::pair<bool, double> Sphere::intersect(Vector origem, Vector direcao){
    Vector temp = Vector::subVector(origem, centro);;
    double a = Vector::dotProduct(direcao, direcao);
    double b = Vector::dotProduct(Vector::scalarProduct(temp, 2), direcao);
    double c = Vector::dotProduct(temp, temp) - (raio * raio);

    // Praticamente bascara
    double delta = b*b - 4*a*c;
    double meio = -b/(2*a);
    if (delta < 0) {
        // cout << "Não possui interscao" << endl; 
        return std::make_pair(false, 0);
    }
    else if (delta == 0) {  
        // cout << "Intersecta em " << meio; 
        return std::make_pair(true, meio);
    }
    else{
        double diferenca = sqrt(delta)/(2*a);
        double t1 = meio - diferenca;
        double t2 = meio + diferenca;
        // cout << "Intersecta tanto em " << t1 << " quanto em " << t2 << endl;
        // cout << "O maximo sendo " << max(t1,t2) << endl;
        
        // Checar qual o menor
        double t;
        if (t1 < t2 && t1 >= 0) t = t1;
        else if (t2 >= 0) t = t2;
        else return std::make_pair(false, -1);
        return std::make_pair(true, t);
    }
}

Vector Sphere::get_color(){
    return color;
}