#ifndef CRIARSVG_H
#define CRIARSVG_H

#include "forma.h"
#include "poligono.h"
#include "visibilidade.h"
#include "lista.h"
#include <stdio.h>

/*
   Módulo responsável pela geração de arquivos SVG.
*/

void svgGeo(const char* caminhoCompleto, Lista* formas);
/*
   Gera um arquivo SVG com todas as formas geométricas após processar um arquivo .geo
*/

void svgQry(const char* caminhoCompleto, Lista* formas, Lista* anteparos);
/*
   Gera um arquivo SVG com o estado do sistema após processar um arquivo .qry
*/

void svgRegiaoVisivel(FILE* svgFile, Poligono regiaoVisivel, const char* corPreench, const char* corBorda);
/*
   Desenha uma região de visibilidade (polígono) no arquivo SVG atual.
*/

void svgAnteparos(FILE* svgFile, Lista* anteparos);
/*
   Desenha todos os anteparos no arquivo SVG atual.
*/

void svgForma(FILE* svgFile, Forma f);
/*
   Adiciona uma única forma ao arquivo SVG aberto.
*/

void svgBombaDestruicao(FILE* svgFile, double x, double y, const char* cor);
/*
   Marca a posição de uma bomba de destruição no SVG.
*/

void svgBombaPintura(FILE* svgFile, double x, double y, const char* cor);
/*
   Marca a posição de uma bomba de pintura no SVG.
*/

void svgBombaClonagem(FILE* svgFile, double x, double y, double dx, double dy);
/*
   Marca a posição de uma bomba de clonagem e mostra o vetor de deslocamento.
*/

void svgTextoRelatorio(FILE* svgFile, double x, double y, const char* texto, const char* cor);
/*
   Adiciona texto de relatório ao SVG para propósitos de depuração ou anotação.
*/

void svgCabecalho(FILE* svgFile, double viewBoxX, double viewBoxY, double viewBoxW, double viewBoxH);
/*
   Escreve o cabeçalho do arquivo SVG com a viewBox especificada.
*/

void svgRodape(FILE* svgFile);
/*
   Escreve o rodapé (tag de fechamento) do arquivo SVG.
*/

#endif
