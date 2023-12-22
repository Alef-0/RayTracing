# RayTracing
Um programa de RayTracing criado em C++ (atualmente rudimentar). <br>
Espera as seguintes entradas.

* **C**: O ponto de origem da câmera.
* **M**: A direção que a câmera aponta.
* **UP**: Onde é a parte de "cima" da câmera, precisa possuir componentes ortornormais a **M** para ser válida.
* **Distância da câmera**: Precisa ser maior que 0 para definir a direção.
* **Altura e Largura** da câmera.

O tipo de entrada precisa ser no formato de 3 números seguidos para as componentes X, Y e Z em ordem. Os componentes no cenário podem ser esferas ou planos, vão ser perguntados a quantidade de cada um desses elementos para serem adicionados.

* Esferas
    *   **Centro**: O ponto que define o seu centro.
    *   **Raio**: O tamanho de seu raio.
    *   **Cor**: Um vetor com 3 valores de 0 a 1 para RGB.
* Plano:
    *   **Ponto**: Um ponto para calcular a orientação do plano.
    *   **Normal**: O vetor que define a orientação do plano.
    *   **Cor**: Um vetor com 3 valores de 0 a 1 para RGB.
* Malha:
    * **Quantidade de triângulos**: Quantos triângulos devem ter.
    * **Quantidade de vertices**: Minimo de 3.
    * **Vertices**: Os pontos (X,Y,Z) dos vertices.
    * **Vertices no triângulo**: Um trio que referencia o indíce 
    (começando com zero) dos vertices anteriores para ser o triângulo.
    * **Cor**: Os triangulos terão cores indivduais. De 0 a 1 para RGB.

Depois vai uma série de perguntas se vai querer rotacionar:
* **Translação**: Disponibilize um dx, dy e dz.
* **Rotação**: Disponibilize o eixo e o angulo em graus.

    
