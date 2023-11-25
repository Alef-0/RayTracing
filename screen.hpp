#ifndef SCREEN
#define SCREEN

#include "base.hpp"
#include "scenes.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <climits>
#include <tuple>

using namespace std;

tuple <bool, double, R3Vector> checar_colisao(vector<Sphere> esferas, vector<Plane> planos, 
    R3Vector origem, R3Vector direcao){
    pair <bool, double> resposta = {false, INT_MAX};
    bool achou; double t; R3Vector color;
    
    // Checar para esferas
    for (auto i = esferas.begin(); i != esferas.end(); i++){
        tie (achou, t) = i->intersect(origem, direcao);
        if (achou && t >= 0 && t < resposta.second){
            resposta = {true, t};
            color = i->get_color();
        }
    }
    // Checar para planos
    for (auto i = planos.begin(); i != planos.end(); i++){
        tie (achou, t) = i->intersect(origem, direcao);
        if (achou && t >= 0 && t < resposta.second){
            resposta = {true, t};
            color = i->get_color();
        }
    }

    return make_tuple(resposta.first, resposta.second, color);
}

vector<vector<R3Vector>> make_screen(R3Vector cima, R3Vector direita, R3Vector origem, R3Vector camera, 
    vector<Sphere> esferas, vector<Plane> planos, int altura, int largura){
    
    // Ver o deslocamento
    R3Vector des_horizontal = scalarProduct(direita,(double) 2/(largura - 1));
    R3Vector des_vertical = scalarProduct(cima, (double) 2/(altura - 1));

    // Iterar e ver as interseções
    R3Vector canto_limite_inferior = subVector(subVector(origem, cima),direita);
    R3Vector canto_atual = canto_limite_inferior;

    // Criando o vetor de pixels
    vector<vector<R3Vector>> colors;
    colors.resize(largura);
    for (int i = 0; i < largura; i++) colors[i].resize(altura);
    double t; bool achou; R3Vector color;

    for (int x = 0; x < largura; x++){
        for (int y = 0; y < altura; y++){
            R3Vector direcao = subVector(canto_atual, camera); // Pegar o vetor direcao
            tie (achou, t, color) = checar_colisao(esferas, planos, canto_atual, direcao); // Checar a colisao com cada um
            if (achou && t >= 0) {colors[x][y] = color;}
            else {colors[x][y] = {0,0,0};}
        
        // Atualizar sem multiplicar
        canto_atual = addVector(canto_atual, des_vertical);
        }
        canto_limite_inferior = addVector(canto_limite_inferior, des_horizontal);
        canto_atual = canto_limite_inferior;
    }

    return colors;
}

void print_ppm(vector<vector<R3Vector>> colors, int altura, int largura){
    fstream file;
    file.open("imagem.ppm", ios::out);
    // Criar a imagem
    file << "P3" << endl;
    file << largura << " " << altura << endl;
    file << 255 << endl;
    clog << endl;
    for (int y = altura - 1; y >= 0; y--){
        clog << "Faltando " << y << " linhas \r" << flush;
        for (int x = 0; x < largura; x++){
            file << colors[x][y].x * 255 << " " << colors[x][y].y * 255 << " " << colors[x][y].z * 255 << " ";
        }
        file << endl;
    }
    
    file.close();
}

#endif