#ifndef TRATANOMEARQUIVO_H
#define TRATANOMEARQUIVO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define PATH_LEN 256
#define FILE_NAME_LEN 100
#define SUFFIX_LEN 50

/*
   Módulo responsável por tratar os nomes dos arquivos durante o processamento do programa.
*/

void criarDiretorioRecursivo(const char* path);
/*
   Cria diretórios recursivamente caso necessário
*/

void extrairNomeBase(const char* caminhoCompleto, char* nomeBase);
/*
   Extrai o nome base dos arquivos .geo/.qry (remove caminho e extensão)
*/

void gerarNomeGeoSvg(const char* nomeBase, const char* outputDir, char* caminhoCompleto);
/*
   Gera o nome do SVG baseado apenas no arquivo .geo (arg.svg)
*/

void gerarNomeQrySvg(const char* nomeBaseGeo, const char* nomeBaseQry, const char* outputDir, char* caminhoCompleto);
/*
   Gera o nome do SVG combinado do arquivo .geo e .qry (arg-arqcons.svg)
*/

void gerarNomeQryTxt(const char* nomeBaseGeo, const char* nomeBaseQry, const char* outputDir, char* caminhoCompleto);
/*
   Gera o nome do TXT combinado do arquivo .geo e .qry (arg-arqcons.txt)
*/

void gerarNomeComSufixo(const char* nomeBaseGeo, const char* nomeBaseQry, const char* sufixo, const char* outputDir, char* caminhoCompleto, int isSvg);
/*
   Gera o nome do arquivo com sufixo para comandos d, p, cln (arg-arqcons-sufx.svg ou .txt)
*/

int criarDiretorioSeNecessario(const char* caminho);
/*
   Cria diretório se necessário, retorna 1 se sucesso, 0 se falha
*/

#endif
