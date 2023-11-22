#include "base.cpp"
// #include "image.cpp"
#include <iostream>
using namespace std;

pair<R3Vector, R3Vector>  pegar_inputs(R3Vector* C, R3Vector* M, R3Vector* UP,
    double* distancia, double* altura, double* largura){
        cout << "Coloque a origem da camera (X,Y,Z): ";
        cin >> C->x >> C->y >> C->z;
        cout << "Coloque o vetor onde a camera aponta (X,Y,Z): ";
        cin >> M->x >> M->y >> M->z; *M = normalize(*M);
        
        R3Vector V;
        while(1){ // Pegar o vetor UP e checar se é válido
            cout << "Coloque o vetor que aponta para cima (X,Y,Z): ";
            cin >> UP->x >> UP->y >> UP->z; *UP = normalize(*UP);
            V = crossProduct(*M, *UP);
            if (tamanho(V) == 0) {cout << "Valor sem componente ortogonal a câmera." << endl;}
            else { V = normalize(V); break;}
        }
        R3Vector U = normalize(crossProduct(V, *M)); 

        cout << "Coloque a distancia da camera: ";
        cin >> *distancia;
        cout << "Coloque agora a largura e altura, em ordem: ";
        cin >> *largura >> *altura;
    
    return make_pair(V, U);
}

int main(){
    R3Vector C, M, UP;
    double distancia, altura, largura;
    pair<R3Vector, R3Vector> res = pegar_inputs(&C, &M, &UP, &distancia, &altura, &largura);
    R3Vector V = res.first, U = res.second;
    R3Vector W = M; // Só para padronizar o nome

    R3Vector origem = addVector(C, scalarProduct(W, distancia)); // Pegar a origem da tela

    // TODO REST
}