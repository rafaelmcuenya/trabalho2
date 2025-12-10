#ifndef SORT_H
#define SORT_H

#include <stdbool.h>

/*
   TAD responsável pelas funções de ordenação.
*/

typedef int (*ComparaFunc)(void* a, void* b);
typedef void (*TrocaFunc)(void* a, void* b);

void insertionSort(void* array, int n, size_t size, ComparaFunc cmp, TrocaFunc troca);
/*
   Ordena um array usando insertion sort.
*/

void mergeSort(void* array, int n, size_t size, ComparaFunc cmp, TrocaFunc troca, int threshold);
/*
   Ordena um array usando merge sort.
*/

void mergeSortRecursivo(void* array, int esq, int dir, size_t size, ComparaFunc cmp, TrocaFunc troca, int threshold);
/*
   Versão recursiva do merge sort para ordenação parcial de arrays.
*/

void merge(void* array, int esq, int meio, int dir, size_t size, ComparaFunc cmp, TrocaFunc troca);
/*
   Realiza o merge de dois subarrays previamente ordenados.
*/

void* mergeSortLista(void* lista, void* (*getProximo)(void* no), void (*setProximo)(void* no, void* proximo), ComparaFunc cmp);
/*
   Ordena uma lista encadeada usando merge sort.
*/

void quickSortStd(void* array, int n, size_t size, ComparaFunc cmp);
/*
   Ordena um array usando qsort.
*/

void sortArrayPonteiros(void** array, int n, ComparaFunc cmp, TrocaFunc troca, int usarMergeSort, int threshold);
/*
   Ordena um array de ponteiros.
*/

bool estaOrdenado(void* array, int n, size_t size, ComparaFunc cmp);
/*
   Verifica se um array está ordenado.
   Retorna 1 se o array estiver ordenado, 0 caso não.
*/

void trocaGenerica(void* a, void* b, size_t size);
/*
   Função genérica para trocar dois elementos de lugar.
*/

#endif 
