#ifndef GEOMETRIA_H
#define GEOMETRIA_H

#include <stdbool.h>
#include "ponto.h"
#include "anteparo.h"
#include <math.h>

/*
   Este TAD cuida de quaisquer operações que envolvam a área da bomba
*/

typedef void* BoundingBox;
typedef void* Vetor2D;


BoundingBox criaBoundingBox(double xmin, double ymin, double xmax, double ymax);
/*
   Cria uma bounding box com os limites fornecidos.
*/

void liberaBoundingBox(BoundingBox bb);
/*
   Libera a memória associada a uma bounding box.
*/

double distanciaEntrePontos(Ponto p1, Ponto p2);
/*
   Calcula a distância euclidiana entre dois pontos.
   Retorna a distância ou -1.0 em caso de erro.
*/

double anguloPontoRelativo(Ponto origem, Ponto p);
/*
   Calcula o ângulo do ponto em relação à origem, medido a partir do eixo X positivo.
   Retorna o ângulo ou -1.0 em caso de erro.
*/

double distanciaPontoAnteparo(Ponto p, Anteparo a);
/*
   Calcula a distância entre um ponto e um anteparo.
   Retorna a menor distância do ponto ao segmento ou -1.0 em caso de erro.
*/

int orientacaoPontos(Ponto a, Ponto b, Ponto c);
/*
   Predicado de orientação 2D (produto vetorial 2D).
   Calcula a orientação dos pontos a, b, c em ordem.
   Retorna:
     -1: ponto c está à direita do segmento ab (a->b)
      0: pontos são colineares
      1: ponto c está à esquerda do segmento ab
   Retorna -2 em caso de erro.
*/

bool pontoNoSegmento(Ponto p, Anteparo a);
/*
   Verifica se um ponto está sobre um anteparo, incluindo extremidades.
   Retorna true se o ponto está no segmento, false caso contrário.
*/

bool segmentosSeInterceptam(Anteparo a1, Anteparo a2, Ponto* interseccao);
/*
   Verifica se dois anteparos/segmentos se interceptam.
   Se interceptarem e 'interseccao' não for NULL, armazena o ponto de interseção.
   O ponto de interseção deve ser liberado pelo chamador.
   Retorna 1 se há interseção, 0 caso contrário.
*/

bool pontoDentroBoundingBox(Ponto p, BoundingBox bb);
/*
   Verifica se um ponto está dentro de uma bounding box (incluindo bordas).
   Retorna 1 se dentro, 0 caso contrário.
*/

bool boundingBoxesSobrepoem(BoundingBox bb1, BoundingBox bb2);
/*
   Verifica se duas bounding boxes se sobrepõem.
   Retorna 1 se há sobreposição, 0 caso contrário.
*/

BoundingBox boundingBoxDeAnteparo(Anteparo a);
/*
   Calcula a bounding box de um anteparo.
   Retorna uma nova bounding box que deve ser liberada posteriormente.
*/

BoundingBox uniaoBoundingBoxes(BoundingBox bb1, BoundingBox bb2);
/*
   Retorna a união de duas bounding boxes.
   Retorna uma nova bounding box que deve ser liberada posteriormente.
*/

int orientacaoCoordenadas(double ax, double ay, double bx, double by, double cx, double cy);
/*
   Função semelhante à de orientação, mas usando coordenadas diretamente.
   Retorna os mesmos valores que orientacaoPontos().
*/

bool interseccaoSegmentosCoordenadas(double x1, double y1, double x2, double y2,double x3, double y3,
double x4, double y4,double* xi, double* yi);
/*
   Função semelhante à de intersecção, mas usando coordenadas diretamente.
   Se houver interseção e xi/yi não forem NULL, armazena as coordenadas.
   Retorna 1 se há interseção, 0 caso contrário.
*/

double produtoVetorial2D(double ax, double ay, double bx, double by, double cx, double cy);
/*
   Calcula o produto vetorial 2D (determinante) dos vetores (b-a) e (c-a).
   Retorna o valor do produto vetorial.
*/

bool pontoVisivelDeOrigem(Ponto origem, Ponto p, Anteparo obstaculo);
/*
   Verifica se um ponto é visível a partir de uma origem, considerando um obstáculo.
   Retorna 1 se visível, 0 se o obstáculo bloqueia a visão.
*/

double distanciaRelativa(Ponto origem, Ponto p1, Ponto p2);
/*
   Compara a distância de dois pontos em relação a uma origem.
   Retorna negativo se p1 está mais próximo, positivo se p2 está mais próximo, zero se estão à mesma distância.
*/

#endif
