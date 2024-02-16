# RayTracing
Um programa de RayTracing criado em C++ (atualmente rudimentar). <br>
Espera as seguintes entradas.

* **C**: O ponto de origem da câmera.
* **M**: A direção que a câmera aponta.
* **UP**: Onde é a parte de "cima" da câmera, precisa possuir componentes ortornormais a **M** para ser válida.
* **Distância da câmera**: Precisa ser maior que 0 para definir a direção.
* **Altura e Largura** da câmera.

Depois precisa das informações relacionadas as fontes de luzes.
* **Luz ambiente**: Com valores de $[0,255]^3$, referencia os valores base para os objetos.
* **Outras fontes**: Quantas fontes, onde estão, e o valor de sua luz.

O tipo de entrada precisa ser no formato de 3 números seguidos para as componentes X, Y e Z em ordem. Os componentes no cenário podem ser esferas ou planos, vão ser perguntados a quantidade de cada um desses elementos para serem adicionados.

Para cada objeto, incluindo triângulos individuais. Precisa ser mandado os seguintes coeficientes:
*   **Ambiente**, **Difusão** e **Especular**: Com os valores $[0,1]^3$.
* **Rugosidade**: Valor para ser calculado com o coeficiente especular.

Tipo de objetos:

* Esferas
    *   **Centro**: O ponto que define o seu centro.
    *   **Raio**: O tamanho de seu raio.
    *   **Coeficientes de Phong**.
* Plano:
    *   **Ponto**: Um ponto para calcular a orientação do plano.
    *   **Normal**: O vetor que define a orientação do plano.
    *   **Coeficientes de Phong**.
* Malha de triângulos:
    * **Quantidade de triângulos**: Quantos triângulos devem ter.
    * **Quantidade de vertices**: Minimo de 3.
    * **Vertices**: Os pontos (X,Y,Z) dos vertices.
    * **Vertices no triângulo**: Um trio que referencia o indíce (começando com zero) dos vertices anteriores para ser o triângulo.
    *   **Coeficientes de Phong**: Vai ser pedido para cada triângulo.

Depois vai uma série de perguntas se vai querer rotacionar:
* **Translação**: Disponibilize um dx, dy e dz.
* **Rotação**: Disponibilize o eixo e o angulo em graus.

    
