#ifndef SHARED
#define SHARED
    #include "base.cpp"
#endif

#include <iostream>
using namespace std;

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
        R3Vector temp = subVector(origem, centro);;
        double a = dotProduct(direcao, direcao);
        double b = dotProduct(scalarProduct(temp, 2), direcao);
        double c = dotProduct(temp, temp) - (raio * raio);

        // Praticamente bascara
        double delta = b*b - 4*a*c;
        double meio = -b/(2*a);
        if (delta < 0) {
            // cout << "Não possui interscao" << endl; 
            return make_pair(false, 0);
        }
        else if (delta == 0) {  
            // cout << "Intersecta em " << meio; 
            return make_pair(true, meio);
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
        if (teste_paralelo == 0) { 
            // cout << "É paralelo, e não se cruza" << endl;
            return make_pair(false, 0);
        }
        else{
            double t = (dotProduct(normal, ponto) - dotProduct(normal, origem))/teste_paralelo;
            // cout << "Ele se cruza em " << t << endl;
            // if (t < 0) cout << "Entretanto é atrás da tela" << endl;
            return make_pair(true, t);
        }
    }

    R3Vector get_color(){
        return color;
    }
};

// int main(){
//     Sphere teste = Sphere(R3Vector{3,0,7}, 7, R3Vector{100,100,100});
//     teste.intersect(R3Vector{0,0,0}, R3Vector{1,0,0});

//     Plane teste = Plane(R3Vector{0,0,0},R3Vector{0,0,1},R3Vector{100,100,100});
//     teste.intersect(R3Vector{0,0,1}, R3Vector{1,0,1});
// }