#ifndef ANTEPARO_H
#define ANTEPARO_H

/*
   Um anteparo é um segmento de reta que bloqueia a área de ação das bombas.
   Um anteparo é criado a partir da transformação de outras formas geométricas quando pintadas com tinta bloqueante.
*/

typedef void* Anteparo;

Anteparo criaAnteparo(int id, double x1, double y1, double x2, double y2, char* cor);
/*
   Cria um anteparo delimitado pelos pontos dados nos parâmetros. 
   Dentre os parâmetros x1, y1, x2 e y2, somente no máximo três podem possuir o mesmo valor, sendo obrigatório, no mínimo, um deles possuir um valor diferente de, pelo menos, um dos outros parâmetros. 
   A cor do anteparo é dada pela cor da forma original.
*/

int idAnteparo(Anteparo a);
/*
   Coleta o ID de um anteparo específico para possíveis manipulações no mesmo, retornando tal identificador.
*/

double getX1Anteparo(Anteparo a);
/*
   Retorna a coordenada X do primeiro ponto do anteparo.
*/

double getY1Anteparo(Anteparo a);
/*
   Retorna a coordenada Y do primeiro ponto do anteparo.
*/

double getX2Anteparo(Anteparo a);
/*
   Retorna a coordenada X do segundo ponto do anteparo.
*/

double getY2Anteparo(Anteparo a);
/*
   Retorna a coordenada Y do segundo ponto do anteparo.
*/

char* getCorAnteparo(Anteparo a);
/*
   Retorna a cor do anteparo, descrita no padrão sRGB de 6 dígitos.
*/

void moveAnteparo(Anteparo a, double dx, double dy);
/*
   Move ambas as extremidades do anteparo em tantas coordenadas no eixo x e no eixo y, valores repassados respectivamente pelos segundo e terceiro parâmetros.
*/

Anteparo clonaAnteparo(Anteparo a, int novoId);
/*
   Cria uma cópia idêntica do anteparo, retornando um ponteiro para o novo anteparo com os mesmos atributos.
*/

int validaAnteparo(void* a);
/*
   Função de validação interna para verificar se o anteparo é válido.
   Retorna 1 se válida, 0 caso contrário.
*/

void setCorAnteparo(Anteparo a, char* novaCor);
/*
   Altera a cor do anteparo.
*/

void liberaAnteparo(Anteparo a);
/*
   Libera toda a memória associada ao anteparo passado como parâmetro.
*/

#endif
