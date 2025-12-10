#ifndef PINTURA_H
#define PINTURA_H

#include <stdio.h>
#include "retangulo.h"
#include "circulo.h"
#include "texto.h"
#include "linha.h"
#include "anteparo.h"
#include "lista.h"

/*
   Módulo responsável pelas operações de transformação de formas geométricas em anteparos pela "pintura".
*/

int pintaRetangulo(Retangulo retangulo, int ids[4], Lista lista);
/*
   Transforma um retângulo em quatro anteparos correspondentes aos seus lados, onde cada lado vira um anteparo.
   Retorna 1 em caso de sucesso, 0 em caso de falha
*/

int pintaLinha(Linha linha, int novoId, Lista lista);
/*
   Transforma uma linha em um anteparo.
   
*/

int pintaTexto(Texto texto, int novoId, Lista lista);
/*
   Transforma um texto em um anteparo, cujo comprimento depende do número de caracteres e da posição da âncora.
   Retorna 1 em caso de sucesso, 0 em caso de falha
*/

int pintaCirculo(Circulo circulo, char direcao, int novoId, Lista lista);

/*
   Transforma um círculo em um anteparo horizontal ou vertical baseado no segundo parâmetro.
   Retorna 1 em caso de sucesso, 0 em caso de falha
*/

#endif
