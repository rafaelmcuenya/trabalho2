#ifndef LEITOR_H
#define LEITOR_H

#include <stdio.h>
#include <stdbool.h>

/*
   O leitor é responsável por processar o .geo e .qyr que definem as formas geométricas e as operações do sistema.
   Ele coordena a criação de formas, anteparos e a execução das bombas.
*/

void inicializarSistema(void);
/*
   Inicializa todos os componentes do sistema.
*/

void finalizarSistema(void);
/*
   Libera toda a memória alocada pelo sistema e finaliza todos os componentes.
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
