#ifndef POLIGONO_H
#define POLIGONO_H

#include <stdio.h>
#include <stdbool.h>
#include "lista.h"
#include "ponto.h"
#include "anteparo.h"
#include "forma.h"

/*
   Um polígono é uma figura geométrica composta por um conjunto ordenado de vértices, conectados entre si por segmentos de reta, formando uma cadeia fechada.
   O polígono deve conter, no mínimo, três vértices para ser considerado válido.
*/

typedef void* Poligono;

typedef enum {
    POLIGONO_SIMPLES,
    POLIGONO_CONVEXO,
    POLIGONO_ESTRELA
} TipoPoligono;

Poligono criaPoligonoVazio(void);
/*
   Cria um polígono vazio, sem vértices. Deve ser preenchido posteriormente com a adição de novos vértices.
*/

Poligono criaPoligonoDeLista(Lista* vertices);
/*
   Cria um polígono copiando todos os pontos de uma lista existente.
   A lista deve conter pelo menos três pontos para formar um polígono válido.
*/

void liberaPoligono(Poligono p);
/*
   Libera toda a memória associada ao polígono, incluindo todos os vértices armazenados.
*/

void adicionaVerticePoligono(Poligono p, Ponto vertice);
/*
   Adiciona um novo vértice ao final da lista de vértices do polígono.
*/

void removeVerticePoligono(Poligono p, int indice);
/*
   Remove o vértice na posição indicada, caso o índice seja válido.
*/

int getNumVertices(Poligono p);
/*
   Retorna o número total de vértices do polígono.
*/

int getNumSegmentos(Poligono p);
/*
   Retorna o número de segmentos do polígono.  
*/

Ponto getVertice(Poligono p, int indice);
/*
   Retorna uma cópia do vértice na posição indicada.
*/

void* getSegmento(Poligono p, int indice);
/*
   Retorna o vértice correspondente ao segmento indicado.
*/

Lista* getVertices(Poligono p);
/*
   Retorna uma lista nova contendo cópias de todos os vértices do polígono.
*/

Lista* getSegmentos(Poligono p);
/*
   Retorna os segmentos do polígono. 
*/

double calculaAreaPoligono(Poligono p);
/*
   Calcula a área do polígono usando a fórmula do polígono simples (Shoelace).
   Retorna 0 caso o polígono não seja válido.
*/

double calculaPerimetro(Poligono p);
/*
   Calcula o perímetro do polígono somando o comprimento de todos os lados.
*/

bool pontoDentroPoligono(Poligono p, Ponto ponto);
/*
   Determina se um ponto está contido no interior do polígono usando o método
   "ray casting".
*/

bool poligonosSeInterceptam(Poligono p1, Poligono p2);
/*
   Verifica se dois polígonos possuem interseção entre si.
*/

bool poligonoContemPoligono(Poligono externo, Poligono interno);
/*
   Verifica se todos os vértices de um polígono estão contidos dentro de outro.
*/

TipoPoligono classificaPoligono(Poligono p);
/*
   Classifica um polígono em simples, convexo ou estrela.
*/

bool ehPoligonoSimples(Poligono p);
/*
   Verifica se o polígono é simples.
*/

bool ehPoligonoConvexo(Poligono p);
/*
   Verifica se o polígono é convexo utilizando o sinal do produto vetorial.
*/

bool ehPoligonoEstrela(Poligono p);
/*
   Verifica se o polígono é do tipo estrela.
*/

Poligono criaPoligonoConvexo(Lista* pontos);
/*
   Cria um polígono convexo utilizando a lista de pontos fornecida.
*/

void desenhaPoligonoSVG(Poligono p, FILE* svgFile, char* corPreench, char* corBorda);
/*
   Desenha o polígono em um arquivo SVG utilizando as coordenadas dos seus vértices.
*/

Poligono transformaAnteparoEmPoligono(Anteparo a);
/*
   Converte um anteparo em um polígono de dois vértices.
*/

Lista* poligonosDeSegmentos(Lista* segmentos);
/*
   Converte uma lista de anteparos em uma lista de polígonos gerados a partir deles.
*/

Poligono interseccaoPoligonos(Poligono p1, Poligono p2);
/*
   Calcula a interseção de dois polígonos caso um esteja completamente contido no outro.
*/

Poligono uniaoPoligonos(Poligono p1, Poligono p2);
/*
   Calcula a união entre dois polígonos, juntando todos os seus vértices.
*/

Poligono diferencaPoligonos(Poligono p1, Poligono p2);
/*
   Retorna a diferença entre dois polígonos caso não se interceptem.
*/

Poligono criaPoligonoDeForma(int tipoForma, void* forma);
/*
   Cria um polígono aproximado a partir de uma forma geométrica genérica.
*/

#endif
