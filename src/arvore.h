#ifndef ARVORE_H
#define ARVORE_H

#include <stdbool.h>

/*
   A árvore é uma estrutura de dados balanceada (AVL) usada para armazenamento e oganização dos elementos.
   Para o projeto, a árvore gerencia qualquer anteparo que vai ser afetado pela bomba.
*/

typedef struct avl_tree Arvore;
typedef struct avl_node NoArv;

Arvore* iniciarArvore(int (*cmp)(void*, void*));
/*
   Inicializa uma nova árvore AVL vazia.
   A função de comparação 'cmp' é obrigatória e define a ordem dos elementos na árvore.
*/

void freeArvore(Arvore* t, void (*liberaInfo)(void*));
/*
   Libera toda a memória associada à árvore.
   Se 'liberaInfo' for fornecida, também libera os dados armazenados nos nós.
*/

bool vaziaAVL(Arvore* t);
/*
   Verifica se a árvore está vazia.
   Retorna true se estiver vazia, false caso contrário.
*/

int getTamAVL(Arvore* t);
/*
   Retorna o número de elementos armazenados na árvore.
*/

int getAltura(Arvore* t);
/*
   Retorna a altura da árvore (número de níveis).
*/

NoArv* getRaiz(Arvore* t);
/*
   Retorna o nó raiz da árvore.
   Retorna NULL se a árvore estiver vazia.
*/

NoArv* getNoEsquerda(NoArv* no);
/*
   Retorna o nó filho esquerdo de um determinado nó.
   Retorna NULL se não houver filho esquerdo.
*/

NoArv* getNoDireita(NoArv* no);
/*
   Retorna o nó filho direito de um determinado nó.
   Retorna NULL se não houver filho direito.
*/

void* getNoInfo(NoArv* no);
/*
   Retorna a informação armazenada em um nó.
   Não remove o elemento da árvore.
*/

NoArv* insereNo(Arvore* t, void* info);
/*
   Insere um novo elemento na árvore, mantendo o balanceamento AVL.
   Retorna o nó inserido, ou NULL em caso de erro.
*/
 
NoArv* removeNo(Arvore* t, void* chave);
/*
   Remove um elemento da árvore com base na chave fornecida.
   A chave é comparada usando a função 'cmp' fornecida na inicialização.
   Mantém o balanceamento AVL.
   Retorna o nó removido, ou NULL se o elemento não for encontrado.
*/
 
NoArv* buscaBinaria(Arvore* t, void* chave);
/*
   Busca um elemento na árvore com base na chave fornecida.
   A chave é comparada usando a função 'cmp' fornecida na inicialização.
   Retorna o nó encontrado, ou NULL se o elemento não existir.
*/
 
void printOrdem(Arvore* t, void (*printInfo)(void*));

/*
   Percorre a árvore em ordem (esquerda, raiz, direita) e aplica a função 
   'printInfo' a cada elemento.
   Útil para debug e visualização dos dados armazenados.
*/
 
#endif 
