// #include "base.cpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include "base.cpp"

using namespace std;

void create_image(vector<vector<R3Vector>> colors, int largura, int altura){
    fstream file;
    file.open("imagem.ppm", ios::out);
    
    // Criar a imagem
    file << "P3" << endl;
    file << largura << " " << altura << endl;
    file << 255 << endl;
    for (int x = 0; x < largura; x++){
        for (int y = 0; y < altura; y++){
            file << colors[x][y].x << " " << colors[x][y].y << " " << colors[x][y].z << " ";
        }
        file << endl;
    }
    
    file.close();
}

int main(){
    int altura = 720, largura = 1280;
    int BACKGROUND_COLOR = 200;
    
    // Criar para testar
    vector<vector<R3Vector>> colors;
    colors.resize(largura);
    for(int x = 0; x < largura ; x++){
        // colors.push_back(vector<R3Vector>{});
        colors[x].resize(altura);
        for (int y = 0; y < altura; y++){
            colors[x][y].x = rand() % 256;
            colors[x][y].y = rand() % 256;
            colors[x][y].z = rand() % 256;
        }
    }

    create_image(colors, largura, altura);
}