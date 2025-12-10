
#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>

/*
   Este módulo serve como armazenamento das formas e anteparos criados ao decorrer da execução do programa.
*/

typedef struct no No;
typedef struct lista Lista;

Lista* iniciaLista(void);
/*
   Cria e inicializa uma nova lista vazia.
*/

bool estaVazia(Lista *l);
/*
   Verifica se uma lista está vazia.
   Retorna 0 se estiver vazia, 1 caso contrário.
*/

int getTamLista(Lista *l);
/*
   Obtém o tamanho atual da lista e retorna tal quantia.
*/

void insereHead(Lista *l, void *novaInfo);
/*
   Insere um novo elemento no início da lista.
*/

void insereTail(Lista *l, void *novaInfo);
/*
   Insere um novo elemento no fim da lista.
*/

void* removeHead(Lista *l);
/*
   Remove e retorna o primeiro elemento da lista.
*/

void* removeTail(Lista *l);
/*
   Remove e retorna o último elemento da lista.
*/

void* getHeadInfo(Lista *l);
/*
   Obtém o primeiro elemento da lista sem removê-lo.
*/

void* getTailInfo(Lista *l);
/*
   Obtém o último elemento da lista sem removê-lo.
*/

No* getHeadNo(Lista *l);
/*
   Obtém o primeiro nó da lista, retornando-o.
*/

No* getTailNo(Lista *l);

/*
   Obtém o último nó da lista, retornando-o.
*/

void* getNoInfo(No *n);
/*
   Obtém o dado armazenado em um nó específico.
*/

No* vaiNoDepois(No *n);
/*
   Avança para o próximo nó a partir de um nó dado, retornando tal ponteiro.
*/

No* vaiNoAntes(No *n);
/*
   Volta para o nó anterior a partir de um nó dado, retornando o ponteiro do nó anterior.
*/

void* procuraLista(Lista *l, void *key, int (*compare)(void *a, void *b));
/*
   Busca um elemento específico na lista por conteúdo.
*/

int temNaLista(Lista *l, void *key, int (*compare)(void *a, void *b));
/*
   Verifica se um elemento está presente na lista.
   Retorna 1 se o elemento estiver na lista, 0 caso não.
*/

int removendoLista(Lista *l, int (*predicate)(void *data), void (*free_data)(void *data));
/*
   Remove todos os elementos que satisfazem uma condição.
*/

void limpaLista(Lista *l, void (*free_data)(void *data));
/*
   Limpa a lista removendo todos os elementos, mas mantendo a estrutura.
*/

void freeLista(Lista *l, void (*free_data)(void *data));

/*
   Libera completamente a lista e seus elementos.
*/

#endif
