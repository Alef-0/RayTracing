// #include "base.cpp"
#include <iostream>
#include <fstream>
#include <vector>
#include "base.cpp"
using namespace std;

void create_image(vector<vector<int>> colors, int largura, int altura){
    fstream file;
    file.open("imagem.ppm", ios::out);
    
    // Criar a imagem
    file << "P3" << endl;
    file << largura << " " << altura << endl;
    file << 255 << endl;
    for (int x = 0; x < largura; x++){
        for (int y = 0; y < altura; y++){
            file << colors[x][y] << " " << colors[x][y] << " " << colors[x][y] << " ";
        }
        file << endl;
    }
    
    file.close();
}

int main(){
    int altura = 720, largura = 1280;
    int BACKGROUND_COLOR = 200;
    // Criar para testar
    vector<vector<int>> colors;
    for(int x = 0; x < largura ; x++){
        vector<int> novo(altura, BACKGROUND_COLOR);
        colors.push_back(novo);
    }

    create_image(colors, largura, altura);
}