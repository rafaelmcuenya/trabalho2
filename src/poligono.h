#ifndef POLIGONO_H
#define POLIGONO_H

#include "ponto.h"
#include "lista.h"

/*
   Um polígono é uma figura geométrica plana definida por uma sequência fechada de segmentos de reta.
*/

typedef void* Poligono;

typedef enum {
    POLIGONO_SIMPLES,    
    POLIGONO_CONVEXO,    
    POLIGONO_ESTRELA      
} TipoPoligono;

Poligono criaPoligonoVazio(void);
/*
   Cria um polígono vazio (sem vértices).
*/

Poligono criaPoligonoDeLista(Lista* vertices);
/*
   Cria um polígono a partir de uma lista de pontos (vértices).
*/

void liberaPoligono(Poligono p);
/*
   Libera toda a memória associada ao polígono.
*/

void adicionaVerticePoligono(Poligono p, Ponto vertice);
/*
   Adiciona um vértice ao final da lista de vértices do polígono.
*/

void adicionaVerticeComDados(Poligono p, Ponto vertice, int id, void* dadoExtra);
/*
   Adiciona um vértice com dados adicionais.
*/

void removeVerticePoligono(Poligono p, int indice);
/*
   Remove o vértice na posição especificada,e reconecta as pontas.
*/

int getNumVertices(Poligono p);
/*
   Retorna o número de vértices do polígono.
*/

int getNumSegmentos(Poligono p);
/*
   Retorna o número de segmentos/arestas do polígono.
*/

Ponto getVertice(Poligono p, int indice);
/*
   Retorna o vértice na posição especificada.
*/

void* getSegmento(Poligono p, int indice);
/*
   Retorna o segmento na posição especificada.
*/

Lista* getVertices(Poligono p);
/*
   Retorna uma lista com todos os vértices do polígono.
*/

Lista* getSegmentos(Poligono p);
/*
   Retorna uma lista com todos os segmentos(arestas) do polígono.
*/

double calculaAreaPoligono(Poligono p);
/*
   Calcula a área do polígono.

*/

double calculaPerimetro(Poligono p);
/*
   Calcula o perímetro do polígono.
*/

bool pontoDentroPoligono(Poligono p, Ponto ponto);
/*
   Verifica se um ponto está dentro do polígono.
   Retorna 1 se o ponto estiver no interior ou na borda.
*/

bool poligonosSeInterceptam(Poligono p1, Poligono p2);
/*
   Verifica se dois polígonos se interceptam.
*/

bool poligonoContemPoligono(Poligono externo, Poligono interno);
/*
   Verifica se o polígono externo contém completamente o interno.
*/

Poligono interseccaoPoligonos(Poligono p1, Poligono p2);
/*
   Calcula a interseção de dois polígonos.
   Retorna novo polígono representando a área comum ou NULL caso não houver.
*/

Poligono uniaoPoligonos(Poligono p1, Poligono p2);
/*
   Calcula a união de dois polígonos.
   Retorna novo polígono representando a área total.
*/

Poligono diferencaPoligonos(Poligono p1, Poligono p2);
/*
   Calcula a diferença de áreas p1 - p2 .
   Retorna novo polígono.
*/

TipoPoligono classificaPoligono(Poligono p);
/*
   Classifica o polígono como simples, convexo ou estrela.
*/

bool ehPoligonoSimples(Poligono p);
/*
   Verifica se o polígono é simples (sem auto-interseções).
*/

bool ehPoligonoConvexo(Poligono p);
/*
   Verifica se o polígono é convexo.
*/

bool ehPoligonoEstrela(Poligono p);
/*
   Verifica se o polígono é em forma de estrela.
*/

Poligono criaPoligonoConvexo(Lista* pontos);
/*
   Cria o fecho convexo de um conjunto de pontos.
   Retorna polígono convexo que envolve todos os pontos.
*/

void* triangulaPoligono(Poligono p);
/*
   Triangula o polígono (divide em triângulos).
   Retorna lista de triângulos ou polígono especial.
*/

void desenhaPoligonoSVG(Poligono p, FILE* svgFile, char* corPreench, char* corBorda);
/*
   Desenha o polígono em um arquivo SVG.
*/

Poligono criaPoligonoDeForma(int tipoForma, void* forma);
/*
   Cria polígono a partir de uma forma geométrica (círculo, retângulo, texto).
*/

Poligono transformaAnteparoEmPoligono(Anteparo a);
/*
   Converte um anteparo (segmento) em um polígono degenerado (segmento como polígono).
*/

Lista* poligonosDeSegmentos(Lista* segmentos);
/*
   Converte uma lista de segmentos em uma lista de polígonos, onde segmentos conectados formam polígonos.
*/

#endif 
