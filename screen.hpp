#ifndef SCREEN
#define SCREEN

#include "base.hpp"
#include "scenes.hpp"
#include "phong.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <climits>
#include <tuple>
#include <limits.h>

using namespace std;
#define WINDOW 1.0
int counter = 0;

struct Recursao{ // Servir pro futuro
    R3Vector ponto;
    R3Vector normal;
    
    Phong colided;
    bool exist;
};

Recursao checar_colisao(vector<Sphere> esferas, vector<Plane> planos, Mesh malha,
    R3Vector origem, R3Vector direcao){
    Recursao resposta; resposta.exist = false; //default
    bool achou; double t;
    double last_t = INT_MAX;
    R3Vector color;
    
    // Checar para esferas
    for (Sphere i : esferas){
        tie (achou, t) = i.intersect(origem, direcao);
        if (achou && t >= 0 && t < last_t){
            last_t = t; resposta.exist = true;
            resposta.colided = i;;
            resposta.ponto = addVector(origem, scalarProduct(direcao, t));
            resposta.normal = normalize(subVector(resposta.ponto, origem)); // caso especial
        }
    }
    // Checar para planos
    for (Plane i : planos){
        tie (achou, t) = i.intersect(origem, direcao);
        if (achou && t >= 0 && t < last_t){
            last_t = t; resposta.exist = true;
            resposta.colided = i;
            resposta.ponto = addVector(origem, scalarProduct(direcao, t));
            resposta.normal = i.normal;
        }
    }
    // Checar para triangulos
    for (auto i : malha.return_triangles()){
        tie(achou, t) = i.intersect(origem, direcao);
        if (achou && t >= 0 && t < last_t){
            last_t = t; resposta.exist = true;
            resposta.colided = i;
            resposta.ponto = addVector(origem, scalarProduct(direcao, t));
            resposta.normal = i.normal;
        }
    }

    return resposta;
}

vector<vector<R3Vector>> make_screen(R3Vector cima, R3Vector direita, R3Vector origem, R3Vector camera, 
    vector<Sphere> esferas, vector<Plane> planos, Mesh triangulos, int altura, int largura,
    R3Vector ambiente, vector<Light> luzes){
    
    // Ver o deslocamento
    R3Vector des_horizontal = scalarProduct(direita,(double) 2 * WINDOW / (largura - 1));
    R3Vector des_vertical = scalarProduct(cima, (double) 2 * WINDOW / (altura - 1));

    // Iterar e ver as interseções
    R3Vector canto_limite_inferior = subVector(subVector(origem, scalarProduct(cima,WINDOW)
                                                ),scalarProduct(direita, WINDOW));
    R3Vector canto_atual = canto_limite_inferior;

    // Criando o vetor de pixels
    vector<vector<R3Vector>> colors;
    colors.resize(largura);
    for (int i = 0; i < largura; i++) colors[i].resize(altura);

    for (int x = 0; x < largura; x++){
        for (int y = 0; y < altura; y++){
            R3Vector direcao = subVector(canto_atual, camera); // Pegar o vetor direcao
            Recursao colisao = checar_colisao(esferas, planos, triangulos, canto_atual, direcao); // Checar a colisao
        
            // Implementando Phong
            if (colisao.exist){
                R3Vector color = phong_pixel(
                    colisao.colided, origem, luzes, ambiente,
                    colisao.ponto, colisao.normal
                );
                // Limitar para dar overflow
                color.x = min(color.x, 255.0);
                color.y = min(color.y, 255.0);
                color.z = min(color.z, 255.0);
                colors[x][y] = color;
            }else{colors[x][y] = {0,0,0};}

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
    string nome = "images/imagem" + to_string(counter++) + ".ppm";
    file.open(nome, ios::out);
    
    // Criar a imagem
    file << "P3" << endl;
    file << largura << " " << altura << endl;
    file << 255 << endl;
    cout << "\nImprimindo\n";
    for (int y = altura - 1; y >= 0; y--){
        // clog << "Faltando " << y << " linhas \r"; // Se tornou muito lento
        for (int x = 0; x < largura; x++){
            file << (int) colors[x][y].x << " " << (int) colors[x][y].y << " " << (int) colors[x][y].z << " ";
        }
        file << "\n";
    }
    file.close();
}

#endif