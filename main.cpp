#include "base.hpp"
#include "scenes.hpp"
#include "screen.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <climits>
#include <tuple>
#include <array>

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

pair<vector<Sphere>, vector<Plane>> pegar_objetos (){
    int quant;
    vector<Sphere> esferas; vector<Plane> planos;

    cout << "Coloque quantas esferas: ";
    cin >> quant; for (int i = 0; i < quant; i++){
        R3Vector centro, color;
        double raio;
        cout << i << "- Coloque o centro da esfera (X,Y,Z): ";
        cin >> centro.x >> centro.y >> centro.z;
        cout << i << "- Coloque Agora o raio: ";
        cin >> raio;
        cout << i << "- Coloque agora a cor (R,G,B) [0,1]: ";
        cin >> color.x >> color.y >> color.z;
        
        esferas.push_back(Sphere{centro,raio,color});
    }

    cout << "Coloque quantos planos: ";
    cin >> quant; for (int i = 0; i < quant; i++){
        R3Vector ponto, color, normal;
        cout << i << "- Agora um ponto do plano (X,Y,Z): ";
        cin >> ponto.x >> ponto.y >> ponto.z;
        cout << i << "- Coloque o vetor normal (X,Y,Z): ";
        cin >> normal.x >> normal.y >> normal.z;
        cout << i << "- Coloque agora a cor (R,G,B) [0,1]: ";
        cin >> color.x >> color.y >> color.z;

        planos.push_back(Plane{ponto,normal,color});
    }

    return make_pair(esferas, planos);
}

Mesh pegar_triangulos(){
    vector<triangle> triangulos;
    vector<R3Vector> vertices;
    vector <array<int,3>> indices;
    
    cout << "Coloque a quantidade de triangulos: ";
    int quant; cin >> quant; if (quant == 0) return Mesh(triangulos, vertices, indices);
    cout << "Coloque quantos vertices: ";
    int quant_vertices; cin >> quant_vertices;
    
    cout << "Coloque as vertices da forma (X,Y,Z): " << endl;
    for(int i = 0; i < quant_vertices; i++){
        R3Vector ponto;
        cin >> ponto.x >> ponto.y >> ponto.z;
        vertices.push_back(ponto);
    }
    
    cout << "Coloque agora os pare de vertice que formam cada triangulo, com o indice de onde eles estao: " << endl;
    for (int i = 0; i < quant; i++){
        int a, b, c;
        cin >> a >> b >> c;
        indices.push_back(array<int,3>{a,b,c});
    }
    cout << "Qual vai ser a cor das meshes (R,G,B) de [0,1]: ";
    vector<R3Vector> cores; 
    R3Vector cor;
    for (int i = 0; i < quant; i++){
        cin >> cor.x >> cor.y >> cor.z;
        cores.push_back(cor);
    }

    // Colocar os triangulos
    int y = 0; // So pra não mudar
    for (array<int,3> i : indices){
        triangulos.push_back(triangle(vertices[i[0]], vertices[i[1]], vertices[i[2]], cores[y++]));
    }

    // Todo calcular a normal dos vertices

    return Mesh(triangulos, vertices, indices);
}

int main(){
    R3Vector C, M, UP;
    double distancia, altura, largura;
    R3Vector V, U;
    tie (V, U) = pegar_inputs(&C, &M, &UP, &distancia, &altura, &largura);
    R3Vector W = M; // Só para padronizar o nome

    R3Vector origem = addVector(C, scalarProduct(W, distancia)); // Pegar a origem da tela

    // Pegar agora os objetos das cenas
    vector<Sphere> esferas; vector<Plane> planos; 
    tie (esferas, planos) = pegar_objetos();
    Mesh malha = pegar_triangulos();

    // Checar agora a interseção e rotações
    char choice;
    while (true){
        vector<vector<R3Vector>> colors = make_screen(U, V, origem, C, esferas, planos, malha ,altura, largura);
        print_ppm(colors, altura, largura);
        cout << "Alguma outra rotacao [y/Y]?\n"; cin >> choice;
        if (choice != 'y' && choice != 'Y') break;
        
        // Especificar rotacao
        cout << "Rotacao [r/R] ou translacao [t/T]?\n"; cin >>choice;
        if (choice == 'r' || choice == 'R'){
            double angulo;
            cout << "Eixo que voce quer rotacionar e o angulo (graus): ";
            cin >> choice >> angulo ;
            if (choice != 'x' && choice != 'y' && choice != 'z') {
                cout << "invalido\n"; break;
            }
            
            // Rotacionar centros
            for (int i = 0; i < esferas.size(); i++){
                esferas[i].auto_rotation(angulo, choice);
            } 
            // Rotacionar planos
            for (int i = 0; i < planos.size(); i++){
                planos[i].auto_rotation(angulo, choice);
            } 
            // Rotacionar triangulos
            vector <triangle>* triangulos = malha.get_triangles(); //Caso contrario seria por valor
            for (int i = 0; i < triangulos->size() ; i++){
                (*triangulos)[i].auto_rotation(angulo, choice);
            }
        }
        else if (choice == 't' || choice == 'T'){
            double dx, dy, dz;
            cout << "Coloque os valores de dx, dy e dz: ";
            cin >> dx >> dy >> dz;
            
            //mover esferas
            for (int i = 0; i < esferas.size(); i++){
                esferas[i].auto_translation(dx,dy,dz);
            }
            // Mover planos
            for (int i = 0; i < planos.size(); i++){
                planos[i].auto_translation(dx,dy,dz);
            }
            // Mover triangulos
            vector <triangle>* triangulos = malha.get_triangles(); // Caso contrario seria por valor
            for (int i = 0; i < triangulos->size() ; i++){
                (*triangulos)[i].auto_translation(dx,dy,dz);
            }
        }
        else break;
    }
}