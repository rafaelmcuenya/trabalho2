#ifndef VISIBILIDADE_H
#define VISIBILIDADE_H

#include "anteparo.h"
#include "lista.h"
#include "ponto.h"
#include "arvore.h"
#include "poligono.h"

/*
   Módulo responsável por calcular a região do plano que é visível a partir de um ponto específico, considerando um conjunto de anteparos que bloqueiam a linha de visão.
*/

typedef void* RegiaoVisivel;

RegiaoVisivel criaRegiaoVisivel(void);
/*
   Cria uma estrutura vazia para armazenar uma região de visibilidade.
   Retorna um ponteiro para a região ou NULL em caso de falha na alocação.
*/

void liberaRegiaoVisivel(RegiaoVisivel regiao);
/*
   Libera toda a memória associada à região de visibilidade.
   Se a região contiver um polígono interno, ele também é liberado.
*/

Poligono calculaRegiaoVisivel(Ponto origem, Lista* anteparos, char tipoOrdenacao, double raioMaximo, int threshold);
/*
   Calcula a região do plano visível a partir do ponto 'origem', considerando os anteparos fornecidos.
   Os anteparos são segmentos que obstruem a visão.
   Retorna um polígono representando a região visível, ou NULL se não houver região visível/caso de erro.
*/

bool pontoDentroRegiaoVisivel(RegiaoVisivel regiao, Ponto p);
/*
   Verifica se um ponto está contido dentro da região de visibilidade calculada.
   Retorna 1 se o ponto está dentro da região, 0 caso contrário.
*/

Poligono getPoligonoRegiaoVisivel(RegiaoVisivel regiao);
/*
   Retorna o polígono que representa a região de visibilidade.
   Retorna NULL se a região não contiver um polígono válido.
*/

Ponto getOrigemRegiaoVisivel(RegiaoVisivel regiao);
/*
   Retorna uma cópia do ponto de origem a partir do qual a região foi calculada.
*/

void desenhaRegiaoVisivelSVG(RegiaoVisivel regiao, FILE* svgFile, const char* corPreench, const char* corBorda);
/*
   Desenha a região de visibilidade em um arquivo SVG.
*/

Ponto raioAteAnteparo(Arvore* segAtivos, Ponto origem, double angulo, double raioMax);
/*
   Função interna que traça um raio a partir da origem em uma direção específica até encontrar um anteparo.
   Usa a árvore AVL de segmentos ativos para encontrar o segmento mais próximo que intercepta o raio.
   Retorna o ponto onde o raio termina.
*/

int comparaEventosVisibilidade(const void* a, const void* b);
/*
   Função interna que atua como comparador para ordenação dos eventos na varredura angular.
   Retorna negativo se a vem antes de b, zero se são iguais, positivo se a vem depois de b.
*/

#endif
