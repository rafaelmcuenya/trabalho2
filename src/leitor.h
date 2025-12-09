#ifndef LEITOR_H
#define LEITOR_H

#include <stdio.h>
#include <stdbool.h>

/*
   O leitor é responsável por processar arquivos de comando (.geo e .qry) que definem as formas geométricas e as operações do sistema.
   Ele coordena a criação de formas, disparadores, carregadores e a execução das operações na arena.
*/

void inicializarSistema(void);
/*
   Inicializa todos os componentes do sistema. Deve ser chamado antes de processar qualquer arquivo.
*/

void finalizarSistema(void);
/*
   Libera toda a memória alocada pelo sistema e finaliza todos os componentes. Deve ser chamado ao término da execução.
*/

double getPontuacaoFinal(void);
/*
   Retorna a pontuação total acumulada de todos os cálculos realizados.
*/

int getTotalInstrucoes(void);
/*
   Retorna o número total de instruções processadas.
*/

int getTotalDisparos(void);
/*
   Retorna o número total de disparos realizados.
*/

int getTotalEsmagadas(void);
/*
   Retorna o número total de formas esmagadas.
*/

int getTotalClonadas(void);
/*
   Retorna o número total de formas clonadas.
*/

void abrirArquivo(FILE **f, const char *caminho);
/*
   Abre um arquivo para leitura, tratando erros de abertura.
*/

void processarComando(const char* linha, int ehQry, const char* nomeBase, const char* outputDir);
/*
   Processa uma linha de comando individual de um arquivo de entrada.
*/

void processarArquivo(const char* caminho, const char* inputDir, int ehQry, const char* nomeBase, const char* outputDir);
/*
   Processa um arquivo completo de comandos de entrada.
*/

#endif
