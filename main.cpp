#include "base.hpp"
#include "classes.hpp"
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

pair<R3Vector, vector<Light>> pegar_luzes(){
    R3Vector ambiente; vector<Light> luzes;
    cout << "Insira a luz ambiente [0,255] em RGB: ";
    cin >> ambiente.x >> ambiente.y >> ambiente.z;
    cout << "Quantas fontes de luzes? ";
    int quant; cin >> quant;
    for (int i = 0; i < quant; i++){
        R3Vector luz, origem;
        cout << "Origem da luz " << i << " : ";
        cin >> origem.x >> origem.y >> origem.z;
        cout << "Cor da luz " << i << " em [0,255] RGB: ";
        cin >> luz.x >> luz.y >> luz.z;
        luzes.push_back(Light(origem, luz));
    }
    return make_pair(ambiente, luzes);
}

pair<vector<Sphere>, vector<Plane>> pegar_objetos (){
    int quant;
    vector<Sphere> esferas; vector<Plane> planos;

    cout << "Coloque quantas esferas: ";
    cin >> quant; for (int i = 0; i < quant; i++){
        R3Vector centro, ka, kd, ks, kr, kt;
        double raio, rug, IOR;
        cout << i << "- Coloque o centro da esfera (X,Y,Z): ";
        cin >> centro.x >> centro.y >> centro.z;
        cout << i << "- Coloque Agora o raio: ";
        cin >> raio;
        cout << i << "- Coloque agora o coeficiente ambiental [0,1]: ";
        cin >> ka.x >> ka.y >> ka.z;
        cout << i << "- Coloque agora o coeficiente difuso [0,1]: ";
        cin >> kd.x >> kd.y >> kd.z;
        cout << i << "- Coloque agora o coeficiente especular [0,1]: ";
        cin >> ks.x >> ks.y >> ks.z;
        cout << i << "- Coloque agora o coeficiente de rugosidade > 0: ";
        cin >> rug;
        cout << i << "- Coloque agora o coeficiente de reflexao: ";
        cin >> kr.x >> kr.y >> kr.z;
        cout << i << "- Coloque agora o coeficiente de transmissao: ";
        cin >> kt.x >> kt.y >> kt.z;
        cout << i << "- Coloque agora o indice de refracao: ";
        cin >> IOR;
        
        esferas.push_back(Sphere{centro, raio, ka, kd, ks, rug, kr, kt, IOR});
    }
    cout << "Coloque quantos planos: ";
    cin >> quant; for (int i = 0; i < quant; i++){
        R3Vector ponto, ka, kd, ks, normal, kr, kt;
        double rug, IOR;
        cout << i << "- Agora um ponto do plano (X,Y,Z): ";
        cin >> ponto.x >> ponto.y >> ponto.z;
        cout << i << "- Coloque o vetor normal (X,Y,Z): ";
        cin >> normal.x >> normal.y >> normal.z;
        cout << i << "- Coloque agora o coeficiente ambiental [0,1]: ";
        cin >> ka.x >> ka.y >> ka.z;
        cout << i << "- Coloque agora o coeficiente difuso [0,1]: ";
        cin >> kd.x >> kd.y >> kd.z;
        cout << i << "- Coloque agora o coeficiente especular [0,1]: ";
        cin >> ks.x >> ks.y >> ks.z;
        cout << i << "- Coloque agora o coeficiente de rugosidade > 0: ";
        cin >> rug;
        cout << i << "- Coloque agora o coeficiente de reflexao: ";
        cin >> kr.x >> kr.y >> kr.z;
        cout << i << "- Coloque agora o coeficiente de transmissao: ";
        cin >> kt.x >> kt.y >> kt.z;
        cout << i << "- Coloque agora o indice de refracao: ";
        cin >> IOR;

        planos.push_back(Plane{ponto,normal, ka, kd, ks, rug, kr, kt, IOR});
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
    
    R3Vector ka, kd, ks, kr, kt;
    double rug, IOR;
    cout << "Coloque agora os pares de vertice que formam cada triangulo, com o indice de onde eles estao, e suas propriedades: " << endl;
    for (int i = 0; i < quant; i++){
        int a, b, c;
        cout << i << "-Coloque os indices: ";
        cin >> a >> b >> c;
        cout << i << "- Coloque agora o coeficiente ambiental [0,1]: ";
        cin >> ka.x >> ka.y >> ka.z;
        cout << i << "- Coloque agora o coeficiente difuso [0,1]: ";
        cin >> kd.x >> kd.y >> kd.z;
        cout << i << "- Coloque agora o coeficiente especular [0,1]: ";
        cin >> ks.x >> ks.y >> ks.z;
        cout << i << "- Coloque agora o coeficiente de rugosidade > 0: ";
        cin >> rug;
        cout << i << "- Coloque agora o coeficiente de reflexao: ";
        cin >> kr.x >> kr.y >> kr.z;
        cout << i << "- Coloque agora o coeficiente de transmissao: ";
        cin >> kt.x >> kt.y >> kt.z;
        cout << i << "- Coloque agora o indice de refracao: ";
        cin >> IOR;
        triangulos.push_back(
            triangle(vertices[a], vertices[b], vertices[c], ka, kd, ks, rug, kr, kt, IOR)
        );
    }
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
    R3Vector ambiente; vector<Light> luzes;
    tie (ambiente, luzes) = pegar_luzes();
    vector<Sphere> esferas; vector<Plane> planos; 
    tie (esferas, planos) = pegar_objetos();
    Mesh malha = pegar_triangulos();

    // Checar agora a interseção e rotações
    char choice;
    Everything world = {esferas, planos, malha.triangulos};
    while (true){
        vector<vector<R3Vector>> colors = make_screen(U, V, origem, C, world,
            altura, largura, ambiente, luzes);
        print_ppm(colors, altura, largura);
        cout << "Alguma outra rotacao [y/Y]?\n"; cin >> choice;
        if (choice != 'y' && choice != 'Y') break;
        
        // Especificar rotacao
        cout << "Rotacao [r/R] ou translacao [t/T]?\n"; cin >>choice;
        if (choice == 'r' || choice == 'R'){
            double angulo;
            cout << "Eixo que voce quer rotacionar e o angulo (graus): ";
            cin >> choice >> angulo ;
            if (choice != 'x' && choice != 'y' && choice != 'z' && choice != 'X' && choice != 'Y' && choice != 'Z') {
                cout << "invalido\n"; break;
            }
            
            for (int i=0;i<world.esferas.size();i++){world.esferas[i].auto_rotation(angulo, choice);} 
            for (int i=0;i<world.planos.size();i++){world.planos[i].auto_rotation(angulo, choice);} 
            for (int i=0;i<world.triangulos.size();i++){world.triangulos[i].auto_rotation(angulo, choice);}
        }
        else if (choice == 't' || choice == 'T'){
            double dx, dy, dz;
            cout << "Coloque os valores de dx, dy e dz: ";
            cin >> dx >> dy >> dz;
            
            for (int i=0;i<world.esferas.size();i++){world.esferas[i].auto_translation(dx,dy,dz);}
            for (int i=0;i<world.planos.size();i++){world.planos[i].auto_translation(dx,dy,dz);}
            for (int i=0;i<world.triangulos.size();i++){world.triangulos[i].auto_translation(dx,dy,dz);}
        }
        else break;
    }
}