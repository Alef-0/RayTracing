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
    *   **Cor**: Um vetor com 3 valores de 0 a 1 para RGB.
    *   **Raio**: O tamanho de seu raio.
* Plano:
    *   **Normal**: O vetor que define a orientação do plano.
    *   **Cor**: Um vetor com 3 valores de 0 a 1 para RGB.
    *   **Ponto**: Um ponto para calcular a orientação do plano.
    
