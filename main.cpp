// Ficava importando base.cpp aqui e no scenes
// Pra corrigir precisa de algo com o ifndef só que não entendi

// #ifndef SHARED
//     #include "base.cpp"
// #endif

#include "scenes.cpp"
#include <iostream>
#include <vector>
#include <fstream>

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
        cout << i << "- Coloque agora a cor (R,G,B): ";
        cin >> color.x >> color.y >> color.z;
        cout << i << "- Agora o raio: ";
        cin >> raio;

        esferas.push_back(Sphere{centro,raio,color});
    }

    cout << "Coloque quantos planos: ";
    cin >> quant; for (int i = 0; i < quant; i++){
        R3Vector ponto, color, normal;
        cout << i << "- Coloque o vetor normal (X,Y,Z): ";
        cin >> normal.x >> normal.y >> normal.z;
        cout << i << "- Coloque agora a cor (R,G,B): ";
        cin >> color.x >> color.y >> color.z;
        cout << i << "- Agora um ponto do plano (X,Y,Z): ";
        cin >> ponto.x >> ponto.y >> ponto.z;

        planos.push_back(Plane{ponto,normal,color});
    }

    return make_pair(esferas, planos);
}

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
    R3Vector des_horizontal = scalarProduct(direita,(double) 2/(largura-1));
    R3Vector des_vertical = scalarProduct(cima, (double) 2/(altura-1));

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
        
            // cout << direcao.x << " "  << direcao.y << " " << direcao.z << " " << endl; 
            // cout << canto_atual.x << " "  << canto_atual.y << " "  << canto_atual.z << endl;
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
    for (int y = altura - 1; y >= 0; y--){
        for (int x = 0; x < largura; x++){
            file << colors[x][y].x * 255 << " " << colors[x][y].y * 255 << " " << colors[x][y].z * 255 << " ";
        }
        file << endl;
    }
    
    file.close();

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

    // Checar agora a interseção
    vector<vector<R3Vector>> colors = make_screen(U, V, origem, C, esferas, planos, altura, largura);
    print_ppm(colors, altura, largura);
}