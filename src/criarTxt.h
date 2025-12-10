#ifndef CRIARTXT_H
#define CRIARTXT_H

#include <stdio.h>
#include "forma.h"
#include "anteparo.h"
#include "visibilidade.h"
#include "poligono.h"

/*
   Este módulo é responsável por criar o arquivo TXT das operações provindas do arquivo .qry.
*/

void iniciarTxt(const char* caminhoCompleto);
/*
   Inicia o arquivo TXT no caminho especificado.
   Se já houver um arquivo aberto, ele é fechado.
*/

void fecharTxt(void);
/*
   Fecha o arquivo TXT atual.
*/

void txtA(int i, int j, char direcao, Forma formaOriginal, Anteparo anteparoGerado);
/*
   Registra a transformação de uma forma em anteparo.
*/

void txtD(double x, double y, const char* sfx, Lista* formasDestruidas, Poligono regiaoVisivel);
/*
   Registra os resultados de uma bomba de destruição.
*/

void txtP(double x, double y, const char* cor, const char* sfx, Lista* formasPintadas, Poligono regiaoVisivel);
/*
   Registra os resultados de uma bomba de pintura.
*/

void txtCln(double x, double y, double dx, double dy, const char* sfx, Lista* formasOriginais, Lista* clones);
/*
   Registra os resultados de uma bomba de clonagem.
*/

void txtComandoQry(const char* linhaComando);
/*
   Copia a linha do comando .qry para o arquivo TXT, precedida por "[*] ".
*/

void txtResposta(const char* resposta);
/*
   Escreve uma linha de resposta/resposta no arquivo TXT.
*/

void txtSeparador(void);
/*
   Insere uma linha separadora (vazia) no arquivo TXT.
*/

void txtFinalRelatorio(int totalFormas, int formasDestruidas, int formasPintadas, int formasClonadas, int anteparosCriados, double areaTotalVisivel);
/*
   Cria o relatório final no TXT com estatísticas gerais.
*/

#endif
