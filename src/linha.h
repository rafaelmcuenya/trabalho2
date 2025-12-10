#ifndef LINHA_H
#define LINHA_H

#include <stdio.h>

/*
   Uma linha é um segmento de reta delimitado por dois pontos, colineares e não sobrepostos, dispostos em um plano. 
   A linha possui uma cor, dada por um código de cor existente no padrão sRGB de 6 dígitos.
*/

typedef void* Linha;

Linha criaLinha(int i, double x1, double y1, double x2, double y2, char* cor);
/*
   Cria uma linha delimitada pelos pontos dados nos parâmetros. 
   Dentre os parâmetros x1, y1, x2 e y2, somente no máximo três podem possuir o mesmo valor, sendo obrigatório, no mínimo, um deles possuir um valor diferente de, pelo menos, um dos outros parâmetros. 
   A cor da linha será dada pelo último parâmetro.
*/

double areaLinha(Linha l);
/*
   Calcula a área teórica de uma linha, utilizando uma fórmula auxiliar dada por A = 2c, onde 'A' é a área final e 'c' o comprimento total da linha. 
   Essa área não é geométrica, servindo apenas como valor teórico para o programa.
*/

int idLinha(Linha l);
/*
   Coleta o ID de uma linha específica para possíveis manipulações no mesmo, retornando tal identificador.
*/

void moveLinha(Linha l, double dx, double dy);
/*
   Move ambas as extremidades da linha em tantas coordenadas no eixo x e no eixo y, valores repassados respectivamente pelos segundo e terceiro parâmetros.
*/

Linha clonaLinha(Linha l, int novoId);
/*
   Cria uma cópia idêntica da linha, retornando um ponteiro para a nova linha com os mesmos atributos.
*/

char* getCorBLinha(Linha l);
/*
   Retorna a cor de borda da linha, descrita no padrão sRGB de 6 dígitos.
    Como a linha possui apenas uma cor, retorna a mesma cordefinida para a linha.
*/

char* getCorPLinha(Linha l);
/*
   Retorna a cor de preenchimento da linha, descrita no padrão sRGB de 6 dígitos.
   Como a linha possui apenas uma cor, retorna a mesma cordefinida para a linha.
*/

double getXLinha(Linha l);
/*
   Retorna a coordenada X da âncora da linha.
*/

double getYLinha(Linha l);
/*
   Retorna a coordenada Y da âncora da linha.
*/

double getX1Linha(Linha l);
/*
   Retorna a coordenada X do primeiro ponto da linha.
*/

double getY1Linha(Linha l);
/*
   Retorna a coordenada Y do primeiro ponto da linha.
*/

double getX2Linha(Linha l);
/*
   Retorna a coordenada X do segundo ponto da linha.
*/

double getY2Linha(Linha l);
/*
   Retorna a coordenada Y do segundo ponto da linha.
*/

char* getCorLinha(Linha l);
/*
   Retorna a cor da linha, descrita no padrão sRGB de 6 dígitos.
*/

int validaLinha(void* l);
/*
   Função de validação interna para verificar se a estrutura da linha é válida.
   Retorna 1 se válida, 0 caso contrário.
*/

void setCorLinha(Linha l, char* novaCor);
/*
   Altera a cor da linha.
*/

void liberaLinha(Linha l);
/*
   Libera toda a memória associada a linha passada como parâmetro.
*/

#endif
