#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
#include "strdupi.h"

typedef struct no {
    void *data;          
    struct no *prev;    
    struct no *next;   
} Node;

struct lista {
    Node *head;          
    Node *tail;         
    int tam;             
};

static Node* criaNode(void *data) {
    Node *novoNode = (Node*)malloc(sizeof(Node));
    if (!novoNode) {
        fprintf(stderr, "Erro: falha na alocação do nó\n");
        return NULL;
    }
    
    novoNode->data = data;
    novoNode->prev = NULL;
    novoNode->next = NULL;
    
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
    
    Node *novoNode = criaNo(novaInfo);
    if (!novoNode) return;
    
    if (estaVazia(l)) {
        l->head = novoNode;
        l->tail = novoNode;
    } else {
        novoNo->next = l->head;
        l->head->prev = novoNode;
        l->head = novoNode;
    }
    
    l->tam++;
}

void insereTail(Lista *l, void *novaInfo) {
    if (!l || !novaInfo) {
        fprintf(stderr, "Erro: parâmetros inválidos em insereTail\n");
        return;
    }
    
    Node *novoNode = criaNo(novaInfo);
    if (!novoNode) return;
    
    if (estaVazia(l)) {
        l->head = novoNode;
        l->tail = novoNode;
    } else {
        novoNo->prev = l->tail;
        l->tail->next = novoNode;
        l->tail = novoNode;
    }
    
    l->tam++;
}

void* removeHead(Lista *l) {
    if (estaVazia(l)) {
        return NULL;
    }
    
    Node *node_to_remove = l->head;
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
    
    Node *node_to_remove = l->tail;
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

No* getHeadNode(Lista *l) {
    return (estaVazia(l)) ? NULL : l->head;
}

No* getTailNode(Lista *l) {
    return (estaVazia(l)) ? NULL : l->tail;
}

void* getNodeInfo(Node *n) {
    return (n == NULL) ? NULL : n->data;
}

No* vaiNodeDepois(Node *n) {
    return (n == NULL) ? NULL : n->next;
}

No* vaiNodeAntes(Node *n) {
    return (n == NULL) ? NULL : n->prev;
}

void* procuraLista(Lista *l, void *key, int (*compare)(void *a, void *b)) {
    if (estaVazia(l) || !key || !compare) {
        return NULL;
    }
    
    Node *current = l->head;
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
    Node *current = l->head;
    Node *next_node;
    
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
    
    Node *current = l->head;
    Node *next_node;
    
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
