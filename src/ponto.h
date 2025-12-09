#ifndef PONTO_H
#define PONTO_H

/*
   Um ponto é um local do espaço, dado por uma coordenada X, e uma coordenada Y, em seus respectivos eixos.
   Qualquer tipo de estrutura geométrica mais complexa, como linhas e formas, são originadas por pontos.
*/

typedef void* Ponto;

Ponto criaPonto(double x, double y);
/*
   Cria um ponto nas coordenadas repassadas pelos parâmetros.
*/

int validaPonto(double x, double y);
/*
   Função para validar a existência de um ponto.
   Retorna 1 se válido, 0 caso contrário.
*/

void movePonto(Ponto p, double x, double y);
/*
   Por meio do ID repassado pelo primeiro parâmetro, move tal ponto em tantas coordenadas no eixo x e eixo y, valores repassados respectivamente pelos segundo e terceiro parâmetros.
*/

double getXPonto(Ponto p);
/*
   Retorna a coordenada X do ponto.
*/

double getYPonto(Ponto p);
/*
   Retorna a coordenada Y do ponto.
*/

Ponto clonaPonto(Ponto p);
/*
   Cria uma cópia do ponto.
*/

void liberaPonto(Ponto p);
/*
   Libera qualquer memória associada ao ponto passado como parâmetro.
   Essa função deve ser utilizada sempre que tal ponto não for mais necessário.
*/

#endif
