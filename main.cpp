#include <iostream>
#include <vector>
#include <fstream>
#include <climits>
#include <tuple>

#include "vector.hpp"
#include "sphere.hpp"
#include "plane.hpp"


std::pair<Vector, Vector>  pegar_inputs(Vector* C, Vector* M, Vector* UP,
    double* distancia, double* altura, double* largura){
        std::cout << "Coloque a origem da camera (X,Y,Z): ";
        std::cin >> C->x >> C->y >> C->z;
        std::cout << "Coloque o vetor onde a camera aponta (X,Y,Z): ";
        std::cin >> M->x >> M->y >> M->z; *M = Vector::normalize(*M);
        
        Vector V;
        while(1){ // Pegar o vetor UP e checar se é válido
            std::cout << "Coloque o vetor que aponta para cima (X,Y,Z): ";
            std::cin >> UP->x >> UP->y >> UP->z; *UP = Vector::normalize(*UP);
            V = Vector::crossProduct(*M, *UP);
            if (Vector::tamanho(V) == 0) {std::cout << "Valor sem componente ortogonal a câmera." << std::endl;}
            else { V = Vector::normalize(V); break;}
        }
        Vector U = Vector::normalize(Vector::crossProduct(V, *M)); 

        std::cout << "Coloque a distancia da camera: ";
        std::cin >> *distancia;
        std::cout << "Coloque agora a largura e altura, em ordem: ";
        std::cin >> *largura >> *altura;
    
    return std::make_pair(V, U);
}

std::pair<std::vector<Sphere>, std::vector<Plane>> pegar_objetos (){
    int quant;
    std::vector<Sphere> esferas; std::vector<Plane> planos;

    std::cout << "Coloque quantas esferas: ";
    std::cin >> quant; for (int i = 0; i < quant; i++){
        Vector centro, color;
        double raio;
        std::cout << i << "- Coloque o centro da esfera (X,Y,Z): ";
        std::cin >> centro.x >> centro.y >> centro.z;
        std::cout << i << "- Coloque Agora o raio: ";
        std::cin >> raio;
        std::cout << i << "- Coloque agora a cor (R,G,B) [0,1]: ";
        std::cin >> color.x >> color.y >> color.z;
        
        esferas.push_back(Sphere(centro,raio,color));
    }

    std::cout << "Coloque quantos planos: ";
    std::cin >> quant; for (int i = 0; i < quant; i++){
        Vector ponto, color, normal;
        std::cout << i << "- Agora um ponto do plano (X,Y,Z): ";
        std::cin >> ponto.x >> ponto.y >> ponto.z;
        std::cout << i << "- Coloque o vetor normal (X,Y,Z): ";
        std::cin >> normal.x >> normal.y >> normal.z;
        std::cout << i << "- Coloque agora a cor (R,G,B) [0,1]: ";
        std::cin >> color.x >> color.y >> color.z;

        planos.push_back(Plane{ponto,normal,color});
    }

    return make_pair(esferas, planos);
}

std::tuple <bool, double, Vector> checar_colisao(std::vector<Sphere> esferas, std::vector<Plane> planos, Vector origem, Vector direcao){
    std::pair <bool, double> resposta = {false, INT_MAX};
    bool achou; double t; Vector color;
    
    // Checar para esferas
    for (auto i = esferas.begin(); i != esferas.end(); i++){
        std::tie (achou, t) = i->intersect(origem, direcao);
        if (achou && t >= 0 && t < resposta.second){
            resposta = {true, t};
            color = i->get_color();
        }
    }
    // Checar para planos
    for (auto i = planos.begin(); i != planos.end(); i++){
        std::tie (achou, t) = i->intersect(origem, direcao);
        if (achou && t >= 0 && t < resposta.second){
            resposta = {true, t};
            color = i->get_color();
        }
    }

    return std::make_tuple(resposta.first, resposta.second, color);
}

std::vector<std::vector<Vector>> make_screen(Vector cima, Vector direita, Vector origem, Vector camera, 
    std::vector<Sphere> esferas, std::vector<Plane> planos, int altura, int largura){
    
    // Ver o deslocamento
    Vector des_horizontal = Vector::scalarProduct(direita,(double) 2/(largura - 1));
    Vector des_vertical = Vector::scalarProduct(cima, (double) 2/(altura - 1));

    // Iterar e ver as interseções
    Vector canto_limite_inferior = Vector::subVector(Vector::subVector(origem, cima),direita);
    Vector canto_atual = canto_limite_inferior;

    // Criando o vetor de pixels
    std::vector<std::vector<Vector>> colors;
    colors.resize(largura);
    for (int i = 0; i < largura; i++) colors[i].resize(altura);
    double t; bool achou; Vector color;

    for (int x = 0; x < largura; x++){
        for (int y = 0; y < altura; y++){
            Vector direcao = Vector::subVector(canto_atual, camera); // Pegar o vetor direcao
            std::tie (achou, t, color) = checar_colisao(esferas, planos, canto_atual, direcao); // Checar a colisao com cada um
            if (achou && t >= 0) {colors[x][y] = color;}
            else {colors[x][y] = {0,0,0};}
        
        // Atualizar sem multiplicar
        canto_atual = Vector::addVector(canto_atual, des_vertical);
        }
        canto_limite_inferior = Vector::addVector(canto_limite_inferior, des_horizontal);
        canto_atual = canto_limite_inferior;
    }

    return colors;
}

void print_ppm(std::vector<std::vector<Vector>> colors, int altura, int largura){
    std::fstream file;
    file.open("../imagem.ppm", std::ios::out);
    // Criar a imagem
    file << "P3" << std::endl;
    file << largura << " " << altura << std::endl;
    file << 255 << std::endl;
    for (int y = altura - 1; y >= 0; y--){
        for (int x = 0; x < largura; x++){
            file << colors[x][y].x * 255 << " " << colors[x][y].y * 255 << " " << colors[x][y].z * 255 << " ";
        }
        file << std::endl;
    }
    
    file.close();
}

int main(){
    Vector C, M, UP;
    double distancia, altura, largura;
    Vector V, U;
    std::tie (V, U) = pegar_inputs(&C, &M, &UP, &distancia, &altura, &largura);
    Vector W = M; // Só para padronizar o nome

    Vector origem = Vector::addVector(C, Vector::scalarProduct(W, distancia)); // Pegar a origem da tela

    // Pegar agora os objetos das cenas
    std::vector<Sphere> esferas; 
    std::vector<Plane> planos;
    std::tie (esferas, planos) = pegar_objetos();

    // Checar agora a interseção
    std::vector<std::vector<Vector>> colors = make_screen(U, V, origem, C, esferas, planos, altura, largura);
    print_ppm(colors, altura, largura);
}