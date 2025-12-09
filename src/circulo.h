#ifndef CIRCULO_H
#define CIRCULO_H

#include <stdio.h>

/*
   Um círculo é uma forma geométrica descrita num plano, onde é composta por infinitos pontos equidistantes de um único e mesmo ponto, dado como centro do círculo, e onde tal distância é denominada raio do círculo.
   As cores de borda e preenchimento são descritas utilizando o padrão sRGB de 6 dígitos.
*/

typedef void* Circulo;

Circulo criaCirculo(int i, double x, double y, double r, char* corb, char* corp);
/*
   Cria um círculo cujo ID é o 'i', com o centro (âncora) dado pelos 2° e 3° parâmetros e com um raio dado pelo quarto parâmetro.
   A cor de borda e preenchimento são dadas pelos dois últimos parâmetros.
   
   ATENÇÃO: Será usado "000000" (preto) para borda e "FFFFFF" (branco) para preenchimento caso ocorra algum erro durante o processo.
*/

double areaCirculo(Circulo c);
/*
   Calcula a área de um círculo com o ID repassado pelo primeiro parâmetro, baseando-se na fórmula geral dada por A = πr², sendo 'r' o raio do círculo, 'A' a área final e pi(π) uma constante universal equivalente a aproximadamente 3,14.
*/

int idCirculo(Circulo c);
/*
   Coleta o ID de um círculo específico para possíveis manipulações no mesmo, retornando tal identificador.
*/

void moveCirculo(Circulo c, double x, double y);
/*
   Por meio do ID repassado pelo primeiro parâmetro, move a âncora de tal círculo em tantas coordenadas no eixo x e eixo y, valores repassados respectivamente pelos segundo e terceiro parâmetros.
*/

Circulo clonaCirculo(Circulo c);
/*
   Cria uma cópia do círculo, retornando um ponteiro para o novo círculo com os mesmos atributos.
*/

double getXCirculo(Circulo c);
/*
   Retorna a coordenada X da âncora do círculo.
*/

double getYCirculo(Circulo c);
/*
   Retorna a coordenada Y da âncora do círculo.
*/

char* getCorBCirculo(Circulo c);
/*
   Retorna a cor de borda do círculo, descrita no padrão sRGB de 6 dígitos.
*/

char* getCorPCirculo(Circulo c);
/*
   Retorna a cor de preenchimento do círculo, descrita no padrão sRGB de 6 dígitos.
*/

double getRaioCirculo(Circulo c);
/*
   Retorna o raio do círculo.
*/

int validaCirculo(void* circ);
/*
   Função interna de validação para verificar se uma estrutura de círculo é válida.
   Retorna 1 se válido, 0 caso contrário.
*/

void setCorBCirculo(Circulo c, char* novaCor);
/*
   Altera a cor da borda do círculo.
*/

void setCorPCirculo(Circulo c, char* novaCor);
/*
   Altera a cor de preenchimento do círculo.
*/

Anteparo criarAnteparoC(Circulo circulo, char direcao, int novo_id);
/*
   Cria um novo anteparo, com um novo ID repassado pelo último parâmetro. Recebendo um círculo por meio do ID do primeiro parâmetro, calcula e transforma num anteparo, usando o segundo parâmetro para indicar a direção (vertical para v, ou horizontal para h).
    A função retorna um ponteiro do novo anteparo criado.
*/

void liberaCirculo(Circulo c);
/*
   Libera toda a memória associada ao círculo passado como parâmetro. Essa função deve ser utilizada sempre que o círculo não for mais necessário.
*/

#endif
