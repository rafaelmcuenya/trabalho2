#ifndef FORMA_H
#define FORMA_H

#include <stdio.h>
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"
#include "anteparo.h"

typedef enum {
    Tc,
    Tr,
    Tl,
    Tt,
    Ta
} TipoForma;

typedef void* Forma;

/*
   Uma forma é uma estrutura genérica utilizada para representar qualquer figura geométrica gerada pelo .geo, podendo ser um círculo, retângulo, linha ou texto. 
   Esse tipo genérico é usado para permitir que as operações do programa possam manipular diferentes figuras de forma independente do tipo específico de cada uma.
*/

Forma criaForma(TipoForma tipo, void* elemento);
/*
   Cria uma nova forma genérica a partir de um tipo específico e do ponteiro para a forma concreta. 
   Essa função encapsula a forma dentro de uma estrutura unificada, permitindo o uso genérico.
*/

TipoForma getTipoForma(Forma f);
/*
   Retorna o tipo da forma passada como parâmetro, permitindo identificar se é um círculo, retângulo, linha ou texto.
*/

int getIdForma(Forma f);
/*
   Retorna o identificador (ID) associado à forma.
*/

double areaForma(Forma f);
/*
   Calcula e retorna a área da forma, independentemente do tipo. 
   A função internamente identifica o tipo da forma e chama o cálculo correspondente.
*/

void moveForma(Forma f, double dx, double dy);
/*
   Move a forma no plano, deslocando sua posição em tantas unidades nos eixos X e Y quanto forem passadas pelos parâmetros. 
   A operação afeta a âncora da figura, dependendo do tipo.
*/

double getXForma(Forma f);
/*
   Retorna a coordenada X da âncora da forma.
*/

double getYForma(Forma f);
/*
   Retorna a coordenada Y da âncora da forma.
*/

char* getCorBForma(Forma f);
/*
   Retorna a cor da borda da forma, descrita no padrão sRGB de 6 dígitos.
*/

char* getCorPForma(Forma f);
/*
   Retorna a cor de preenchimento da forma, descrita no padrão sRGB de 6 dígitos.
*/

int validaCirculo(void* circ);
/*
   Função interna de validação para verificar se uma estrutura de círculo é válida.
   Retorna 1 se válido, 0 caso contrário.
*/

int validaRetangulo(void* ret);
/*
   Função interna de validação para verificar se uma estrutura de retângulo é válida.
   Retorna 1 se válido, 0 caso contrário.
*/

int validaLinha(void* lin);
/*
   Função interna de validação para verificar se uma estrutura de linha é válida.
   Retorna 1 se válida, 0 caso contrário.
*/

int validaTexto(void* txt);
/*
   Função interna de validação para verificar se uma estrutura de texto é válida.
   Retorna 1 se válido, 0 caso contrário.
*/

Circulo getCirculoFromForma(Forma f);
/*
   Retorna o elemento Circulo contido na Forma.
   ATENÇÃO: Apenas para formas do tipo Tc.
*/

Retangulo getRetanguloFromForma(Forma f);
/*
   Retorna o elemento Retangulo contido na Forma.
   ATENÇÃO: Apenas para formas do tipo Tr.
*/

Linha getLinhaFromForma(Forma f);
/*
   Retorna o elemento Linha contida na Forma.
   ATENÇÃO: Apenas para formas do tipo Tl.
*/

Texto getTextoFromForma(Forma f);
/*
   Retorna o elemento Texto contido na Forma.
   ATENÇÃO: Apenas para formas do tipo Tt.
*/

Anteparo getAnteparoFromForma(Forma f);
/*
   Retorna o elemento Anteparo contido na Forma.
   ATENÇÃO: Apenas para formas do tipo Ta.
*/

void freeForma(Forma f);
/*
   Libera toda a memória associada à forma passada como parâmetro, de acordo com o tipo.
   Essa função deve ser utilizada sempre que a forma não for mais necessária.
*/

#endif
