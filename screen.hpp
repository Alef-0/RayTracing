#ifndef SCREEN
#define SCREEN

#include "base.hpp"
#include "classes.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <climits>
#include <tuple>
#include <limits.h>

using namespace std;
#define WINDOW 1.0
int counter = 0;
#define LIMIT 1e-6 // Tentativa de suavizar as sombras
#define DEPTH 3

struct Recursao{ // Servir pro futuro
    R3Vector ponto;
    R3Vector normal;
    
    Phong colided;
    bool exist;
    bool plane;

    double dist;
    int teste;
};

bool bloqueada(Everything world, R3Vector alvo, Light luz, R3Vector normal){ // Sombra esquece
    double t; bool achou;  
    R3Vector direcao = subVector(luz.origem, alvo);
    double distance = tamanho(direcao);
    direcao = normalize(direcao);
    for (Sphere i : world.esferas){ // Esferas
        tie (achou, t) = i.intersect(alvo,direcao);
        if (achou && t > LIMIT && tamanho(scalarProduct(direcao, t))<=distance) 
            return true;
    }
    for (Plane i : world.planos){ // Planos
        tie (achou, t) = i.intersect(alvo,direcao);
        if (achou && t > LIMIT && tamanho(scalarProduct(direcao, t))<=distance) 
            return true;
    }
    for (triangle i : world.triangulos){ // Triangulos
        tie (achou, t) = i.intersect(alvo,direcao);
        if (achou && t > LIMIT && tamanho(scalarProduct(direcao, t))<=distance) 
            return true;
    }
    return false;
}

Recursao checar_colisao(Everything world,R3Vector origem, R3Vector direcao){
    Recursao resposta; resposta.exist = false; //default
    bool achou; double t;
    double last_t = INT_MAX;
    R3Vector color;
    
    int contar = 0; // Checar objetos
    // Checar para esferas
    for (Sphere i : world.esferas){
        tie (achou, t) = i.intersect(origem, direcao);
        R3Vector ponto = addVector(origem, scalarProduct(direcao, t));
        double dist = distancia(ponto, origem);
        if (achou && t >= 0 && t < last_t && dist > LIMIT){
            last_t = t; resposta.exist = true;
            resposta.colided = i;
            resposta.ponto = ponto;
            resposta.normal = normalize(subVector(resposta.ponto, i.centro)); // caso especial
            // Ver a direcao certa
            if (dotProduct(resposta.normal, direcao) > 0) {
                resposta.normal = scalarProduct(resposta.normal, -1.0);
            }
            resposta.plane = false; resposta.dist = dist;
            resposta.teste = contar;
        }
    }
    // Checar para planos
    for (Plane i : world.planos){
        tie (achou, t) = i.intersect(origem, direcao);
        R3Vector ponto = addVector(origem, scalarProduct(direcao, t));
        double dist = distancia(ponto, origem);
        if (achou && t >= 0 && t < last_t && dist > LIMIT){
            last_t = t; resposta.exist = true;
            resposta.colided = i;
            resposta.ponto = ponto;
            // Ver a direcao certa
            if (dotProduct(i.normal, direcao) > 0) resposta.normal = scalarProduct(i.normal, -1.0);
            resposta.normal = i.normal;
            resposta.plane = true; resposta.dist = dist;
            resposta.teste = contar;
        }
    }
    // Checar para triangulos
    for (auto i : world.triangulos){
        tie(achou, t) = i.intersect(origem, direcao);
        R3Vector ponto = addVector(origem, scalarProduct(direcao, t));
        double dist = distancia(ponto, origem);
        if (achou && t >= 0 && t < last_t && dist > LIMIT){
            last_t = t; resposta.exist = true;
            resposta.colided = i;
            resposta.ponto = ponto;
            if (dotProduct(i.normal, direcao) > 0) resposta.normal = scalarProduct(i.normal, -1.0);
            resposta.normal = i.normal;
            resposta.plane = true; resposta.dist = dist;
            resposta.teste = contar;
        }
    }

    return resposta;
}

R3Vector phong_pixel(Recursao alvo, R3Vector origem, vector<Light> luzes, 
                    R3Vector luz_ambiente, Everything world, int depth){

    R3Vector answer;
    R3Vector Viewer = normalize(subVector(origem, alvo.ponto)); // Calcular vetores de direcao da pessoa
    answer = simpleProduct(luz_ambiente, alvo.colided.k_ambiente); // Calcular ambiente

    // Somar tudo
    for (Light luz : luzes){
        R3Vector L_direcao = normalize(subVector(luz.origem, alvo.ponto));
        R3Vector R = reflect(alvo.normal, L_direcao);
        // Tentativa de fazer sombra
        // if (bloqueada(world, alvo, luz, normal) || dotProduct(normal, L_direcao) < LIMIT){continue;} // Não quero pensar em problemas
        
        // Difusao
        double LN = max(0.0, dotProduct(L_direcao, alvo.normal));
        R3Vector difusao = simpleProduct(alvo.colided.k_difuso, luz.intensidade);
        difusao = scalarProduct(difusao, LN);
        answer = addVector(answer, difusao);
        
        // Especular
        R3Vector especular = simpleProduct(alvo.colided.k_especular, luz.intensidade);
        double RVa = pow(max(0.0, dotProduct(R, Viewer)), alvo.colided.rugosidade);
        especular = scalarProduct(especular, RVa);
        answer = addVector(especular, answer);
    }
    // Recursão
    if (depth < DEPTH){
        // Reflexao
        R3Vector reflected_vision = reflect(alvo.normal, Viewer);
        Recursao checar = checar_colisao(world, alvo.ponto, reflected_vision);
        if (checar.exist){
            // if (checar.dist < 1 && checar.teste == alvo.teste){
            //     printf("%lf %lf %lf ||| %lf %lf %lf\n", checar.ponto.x, checar.ponto.y, checar.ponto.z, alvo.ponto.x, alvo.ponto.y, alvo.ponto.z);
            // }
            R3Vector color = phong_pixel(checar, alvo.ponto, luzes, luz_ambiente, world, depth + 1);
            answer = addVector(answer, color);
        }
    }

    return answer;
}

vector<vector<R3Vector>> make_screen(R3Vector cima, R3Vector direita, R3Vector origem, R3Vector camera, 
    Everything world, int altura, int largura, R3Vector ambiente, vector<Light> luzes){
    
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
            Recursao colisao = checar_colisao(world, canto_atual, direcao); // Checar a colisao
        
            // Implementando Phong
            if (colisao.exist){
                R3Vector color = phong_pixel(
                    colisao, origem, luzes, ambiente,
                    world, 0
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