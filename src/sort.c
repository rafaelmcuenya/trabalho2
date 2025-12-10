#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "sort.h"
#include "strdupi.h"

static void mergeArrays(void* array, int esq, int meio, int dir, size_t size, ComparaFunc cmp, TrocaFunc troca);
static void* mergeListas(void* esq, void* dir, ComparaFunc cmp, void* (*getProximo)(void*), void (*setProximo)(void*, void*));

void trocaGenerica(void* a, void* b, size_t size) {
    if (!a || !b || size == 0) return;
    
    char* temp = (char*)malloc(size);
    if (!temp) {
        fprintf(stderr, "Erro: falha na alocação em trocaGenerica\n");
        return;
    }
    
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
    
    free(temp);
}

void insertionSort(void* array, int n, size_t size, ComparaFunc cmp, TrocaFunc troca) {
    if (!array || n <= 1 || !cmp) return;
    
    char* base = (char*)array;
    
    for (int i = 1; i < n; i++) {
        int j = i;
        
        char* chave = (char*)malloc(size);
        if (!chave) {
            fprintf(stderr, "Erro: falha na alocação em insertionSort\n");
            return;
        }
        memcpy(chave, base + i * size, size);
        
        while (j > 0 && cmp(base + (j-1) * size, chave) > 0) {
            if (troca) {
                troca(base + j * size, base + (j-1) * size);
            } else {
                memcpy(base + j * size, base + (j-1) * size, size);
            }
            j--;
        }
        
        if (troca && j != i) {
            memcpy(base + j * size, chave, size);
        } else if (!troca) {
            memcpy(base + j * size, chave, size);
        }
        
        free(chave);
    }
}

void merge(void* array, int esq, int meio, int dir, size_t size, ComparaFunc cmp, TrocaFunc troca) {
    int n1 = meio - esq + 1;
    int n2 = dir - meio;
    char* L = (char*)malloc(n1 * size);
    char* R = (char*)malloc(n2 * size);
    
    if (!L || !R) {
        fprintf(stderr, "Erro: falha na alocação em merge\n");
        if (L) free(L);
        if (R) free(R);
        return;
    }
    
    char* base = (char*)array;
    memcpy(L, base + esq * size, n1 * size);
    memcpy(R, base + (meio + 1) * size, n2 * size);
    
    int i = 0, j = 0, k = esq;
    
    while (i < n1 && j < n2) {
        if (cmp(L + i * size, R + j * size) <= 0) {
            memcpy(base + k * size, L + i * size, size);
            i++;
        } else {
            memcpy(base + k * size, R + j * size, size);
            j++;
        }
        k++;
    }
  
    while (i < n1) {
        memcpy(base + k * size, L + i * size, size);
        i++;
        k++;
    }
    
    while (j < n2) {
        memcpy(base + k * size, R + j * size, size);
        j++;
        k++;
    }
    
    free(L);
    free(R);
}

void mergeSortRecursivo(void* array, int esq, int dir, size_t size, ComparaFunc cmp, TrocaFunc troca, int threshold) {
    if (esq < dir) {
        if (dir - esq + 1 <= threshold) {
            insertionSort((char*)array + esq * size, dir - esq + 1, size, cmp, troca);
            return;
        }
        
        int meio = esq + (dir - esq) / 2;
        
        mergeSortRecursivo(array, esq, meio, size, cmp, troca, threshold);
        mergeSortRecursivo(array, meio + 1, dir, size, cmp, troca, threshold);
        
        merge(array, esq, meio, dir, size, cmp, troca);
    }
}

void mergeSort(void* array, int n, size_t size, ComparaFunc cmp, TrocaFunc troca, int threshold) {
    if (!array || n <= 1 || !cmp) return;
    
    if (threshold <= 0) {
        threshold = 10; 
    }
    
    mergeSortRecursivo(array, 0, n - 1, size, cmp, troca, threshold);
}

static void* mergeListas(void* esq, void* dir, ComparaFunc cmp,
                        void* (*getProximo)(void*),
                        void (*setProximo)(void*, void*)) {
    if (!esq) return dir;
    if (!dir) return esq;
    
    void* resultado = NULL;
    
    if (cmp(esq, dir) <= 0) {
        resultado = esq;
        void* prox = getProximo(esq);
        void* merged = mergeListas(prox, dir, cmp, getProximo, setProximo);
        setProximo(resultado, merged);
    } else {
        resultado = dir;
        void* prox = getProximo(dir);
        void* merged = mergeListas(esq, prox, cmp, getProximo, setProximo);
        setProximo(resultado, merged);
    }
    
    return resultado;
}

void* mergeSortLista(void* lista, 
                     void* (*getProximo)(void* no),
                     void (*setProximo)(void* no, void* proximo),
                     ComparaFunc cmp) {
    if (!lista || !getProximo || !cmp) return lista;
    
    void* meio = lista;
    void* rapido = getProximo(lista);
    void* lento = lista;
    
    while (rapido) {
        rapido = getProximo(rapido);
        if (rapido) {
            rapido = getProximo(rapido);
            lento = getProximo(lento);
        }
    }
    
    meio = getProximo(lento);
    setProximo(lento, NULL);
    void* esq = mergeSortLista(lista, getProximo, setProximo, cmp);
    void* dir = mergeSortLista(meio, getProximo, setProximo, cmp);
    return mergeListas(esq, dir, cmp, getProximo, setProximo);
}

void quickSortStd(void* array, int n, size_t size, ComparaFunc cmp) {
    if (!array || n <= 1 || !cmp) return;
    
    qsort(array, n, size, (int (*)(const void*, const void*))cmp);
}

void sortArrayPonteiros(void** array, int n, ComparaFunc cmp, TrocaFunc troca, 
                        int usarMergeSort, int threshold) {
    if (!array || n <= 1 || !cmp) return;
    
    if (usarMergeSort) {
        mergeSort(array, n, sizeof(void*), cmp, troca, threshold);
    } else {
        quickSortStd(array, n, sizeof(void*), cmp);
    }
}

bool estaOrdenado(void* array, int n, size_t size, ComparaFunc cmp) {
    if (!array || n <= 1 || !cmp) return true;
    
    char* base = (char*)array;
    
    for (int i = 0; i < n - 1; i++) {
        if (cmp(base + i * size, base + (i + 1) * size) > 0) {
            return false;
        }
    }
    return true;
}
