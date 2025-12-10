#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
#include "strdupi.h"

typedef struct no {
    void *data;          
    struct no *prev;    
    struct no *next;   
} No;

struct lista {
    No *head;          
    No *tail;         
    int tam;             
};

static No* criaNo(void *data) {
    No *novoNo = (No*)malloc(sizeof(No));
    if (!novoNo) {
        fprintf(stderr, "Erro: falha na alocação do nó\n");
        return NULL;
    }
    
    novoNo->data = data;
    novoNo->prev = NULL;
    novoNo->next = NULL;
    
    return novoNo;
}

Lista* iniciaLista(void) {
    Lista *l = (Lista*)malloc(sizeof(Lista));
    if (!l) {
        fprintf(stderr, "Erro: falha na alocação da lista\n");
        return NULL;
    }
    
    l->head = NULL;
    l->tail = NULL;
    l->tam = 0;
    
    return l;
}

bool estaVazia(Lista *l) {
    return (l == NULL || l->tam == 0);
}

int getTamLista(Lista *l) {
    return (l == NULL) ? 0 : l->tam;
}

void insereHead(Lista *l, void *novaInfo) {
    if (!l || !novaInfo) {
        fprintf(stderr, "Erro: parâmetros inválidos em insereHead\n");
        return;
    }
    
    No *novoNo = criaNo(novaInfo);
    if (!novoNo) return;
    
    if (estaVazia(l)) {
        l->head = novoNo;
        l->tail = novoNo;
    } else {
        novoNo->next = l->head;
        l->head->prev = novoNo;
        l->head = novoNo;
    }
    
    l->tam++;
}

void insereTail(Lista *l, void *novaInfo) {
    if (!l || !novaInfo) {
        fprintf(stderr, "Erro: parâmetros inválidos em insereTail\n");
        return;
    }
    
    No *novoNo = criaNo(novaInfo);
    if (!novoNo) return;
    
    if (estaVazia(l)) {
        l->head = novoNo;
        l->tail = novoNo;
    } else {
        novoNo->prev = l->tail;
        l->tail->next = novoNo;
        l->tail = novoNo;
    }
    
    l->tam++;
}

void* removeHead(Lista *l) {
    if (estaVazia(l)) {
        return NULL;
    }
    
    No *node_to_remove = l->head;
    void *data = node_to_remove->data;
    
    if (l->tam == 1) {
        l->head = NULL;
        l->tail = NULL;
    } else {
        l->head = node_to_remove->next;
        l->head->prev = NULL;
    }
    
    free(node_to_remove);
    l->tam--;
    
    return data;
}

void* removeTail(Lista *l) {
    if (estaVazia(l)) {
        return NULL;
    }
    
    No *node_to_remove = l->tail;
    void *data = node_to_remove->data;
    
    if (l->tam == 1) {
        l->head = NULL;
        l->tail = NULL;
    } else {
        l->tail = node_to_remove->prev;
        l->tail->next = NULL;
    }
    
    free(node_to_remove);
    l->tam--;
    
    return data;
}

void* getHeadInfo(Lista *l) {
    return (estaVazia(l)) ? NULL : l->head->data;
}

void* getTailInfo(Lista *l) {
    return (estaVazia(l)) ? NULL : l->tail->data;
}

No* getHeadNo(Lista *l) {
    return (estaVazia(l)) ? NULL : l->head;
}

No* getTailNo(Lista *l) {
    return (estaVazia(l)) ? NULL : l->tail;
}

void* getNoInfo(No *n) {
    return (n == NULL) ? NULL : n->data;
}

No* vaiNoDepois(No *n) {
    return (n == NULL) ? NULL : n->next;
}

No* vaiNoAntes(No *n) {
    return (n == NULL) ? NULL : n->prev;
}

void* procuraLista(Lista *l, void *key, int (*compare)(void *a, void *b)) {
    if (estaVazia(l) || !key || !compare) {
        return NULL;
    }
    
    No *current = l->head;
    while (current) {
        if (compare(current->data, key) == 0) {
            return current->data;
        }
        current = current->next;
    }
    
    return NULL;
}

int temNaLista(Lista *l, void *key, int (*compare)(void *a, void *b)) {
    return (procuraLista(l, key, compare) != NULL) ? 1 : 0;
}

int removendoLista(Lista *l, int (*predicate)(void *data), void (*free_data)(void *data)) {
    if (estaVazia(l) || !predicate) {
        return 0;
    }
    
    int removed = 0;
    No *current = l->head;
    No *next_node;
    
    while (current) {
        next_node = current->next;
        
        if (predicate(current->data)) {
            if (current->prev) {
                current->prev->next = current->next;
            } else {
                l->head = current->next;
            }
            
            if (current->next) {
                current->next->prev = current->prev;
            } else {
                l->tail = current->prev;
            }
            
            if (free_data) {
                free_data(current->data);
            }
            free(current);
            
            l->tam--;
            removed++;
        }
        
        current = next_node;
    }
    
    return removed;
}

void limpaLista(Lista *l, void (*free_data)(void *data)) {
    if (!l) return;
    
    No *current = l->head;
    No *next_node;
    
    while (current) {
        next_node = current->next;
        
        if (free_data) {
            free_data(current->data);
        }
        free(current);
        
        current = next_node;
    }
    
    l->head = NULL;
    l->tail = NULL;
    l->tam = 0;
}

void freeLista(Lista *l, void (*free_data)(void *data)) {
    if (!l) return;
    
    limpaLista(l, free_data);
    free(l);
}
