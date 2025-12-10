#ifndef TEXTO_H
#define TEXTO_H

#include <stdio.h>

/*
   Um texto é um conjunto de caracteres exibidos sobre um plano de forma linear. Tal estrutura possui uma âncora, que pode ser posicionada no começo do texto (i), no meio dele (m) ou em seu final (f).
   As cores de borda e preenchimento são descritas utilizando o padrão sRGB de 6 dígitos.
*/

typedef void* Texto;

Texto criaTexto(int i, double x, double y, char* corb, char* corp, char a, char* txto);
/*
   Cria um texto contendo os caracteres do último parâmetro, com a âncora posicionada baseada no caractere recebido pelo sexto parâmetro. 
   Os 2° e 3° parâmetros são a posição da âncora do texto sobre o plano. 
   As cores padrão de preenchimento e borda são dadas pelos quarto e quinto parâmetros.
*/

double areaTexto(Texto t);
/*
   Calcula uma área teórica associada ao texto, apenas para fins de padronização do sistema, dada por 20 vezes o número de characteres do texto.
   A área é estimada de forma simbólica, não geométrica, representando o espaço ocupado pelo texto no plano.
*/

int idTexto(Texto t);
/*
   Coleta o ID de um texto específico para possíveis manipulações no mesmo, retornando tal identificador.
*/

void moveTexto(Texto t, double dx, double dy);
/*
   Por meio do ID repassado pelo primeiro parâmetro, move a âncora do texto selecionado 
   em tantas coordenadas no eixo x e no eixo y, valores repassados respectivamente pelos segundo e terceiro parâmetros.   
*/

char* getTexto(Texto t);
/*
   Retorna a sequência de caracteres contida no texto selecionado.
*/

char getAncoraTexto(Texto t);
/*
   Retorna o tipo de âncora (i, m ou f) associada ao texto.
*/

double getXTexto(Texto t);
/*
   Retorna a coordenada X da âncora do texto.
*/

double getYTexto(Texto t);
/*
   Retorna a coordenada Y da âncora do texto.
*/

char* getCorBTexto(Texto t);
/*
   Retorna a cor de borda do texto, descrita no padrão sRGB de 6 dígitos.
*/

char* getCorPTexto(Texto t);
/*
   Retorna a cor de preenchimento do texto, descrita no padrão sRGB de 6 dígitos.
*/

void setAncoraTexto(Texto t, char ancora);
/*
   Define o tipo de âncora para o texto.
*/

void aplicaStyleTexto(Texto t, char* fontFamily, char* fontWeight, double fontSize);
/*
   Aplica configurações de estilo de fonte ao texto conforme comando ts.
   fontFamily: "sans", "serif", "cursive"
   fontWeight: "n" (normal), "b" (bold), "b+" (bolder), "l" (lighter)
   fontSize: tamanho da fonte em pontos (px)
*/

char* getFontFamilyTexto(Texto t);
/*
   Retorna a família de fonte atual do texto.
*/

char* getFontWeightTexto(Texto t);
/*
   Retorna o peso da fonte atual do texto.
*/

double getFontSizeTexto(Texto t);
/*
   Retorna o tamanho da fonte atual do texto.
*/

int validaTexto(void* t);
/*
   Função de validação interna para verificar se a estrutura do texto é válida.
   Retorna 1 se válido, 0 caso contrário.
*/

void setCorBTexto(Texto t, char* novaCor);
/*
   Altera a cor da borda do texto.
*/

void setCorPTexto(Texto t, char* novaCor);
/*
   Altera a cor de preenchimento do texto.
*/

Texto clonaTexto(Texto t, int novoId);
/*
   Cria uma cópia idêntica do texto.
*/

int getNumCaracteresTexto(Texto t);
/*
   Retorna o número de caracteres do texto.
*/

double getComprimentoTexto(Texto t);
/*
   Retorna o comprimento estimado do texto (10.0 * número de caracteres).
*/

void liberaTexto(Texto t);
/*
   Libera toda a memória associada ao texto passado como parâmetro.
*/

#endif
