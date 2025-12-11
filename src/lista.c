#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

struct No {
    void *data;
    struct No *prev;
    struct No *next;
};

struct Lista {
    struct No *head;
    struct No *tail;
    int tam;
};

static struct No* criaNode(void *data) {
    struct No *novoNode = (struct No*)malloc(sizeof(struct No));
    if (!novoNode) {
        fprintf(stderr, "Erro: falha na alocação do nó\n");
        return NULL;
    }
    
    novoNode->data = data;
    novoNode->prev = NULL;
    novoNode->next = NULL;
    
    return novoNode;
}

Lista* iniciaLista(void) {
    struct Lista *l = (struct Lista*)malloc(sizeof(struct Lista));
    if (!l) {
        fprintf(stderr, "Erro: falha na alocação da lista\n");
        return NULL;
    }
    
    l->head = NULL;
    l->tail = NULL;
    l->tam = 0;
    
    return (Lista*)l;
}

bool estaVazia(Lista *l) {
    struct Lista *lista = (struct Lista*)l;
    return (lista == NULL || lista->tam == 0);
}

int getTamLista(Lista *l) {
    struct Lista *lista = (struct Lista*)l;
    return (lista == NULL) ? 0 : lista->tam;
}

void insereHead(Lista *l, void *novaInfo) {
    struct Lista *lista = (struct Lista*)l;
    
    if (!lista || !novaInfo) {
        fprintf(stderr, "Erro: parâmetros inválidos em insereHead\n");
        return;
    }
    
    struct No *novoNode = criaNode(novaInfo);
    if (!novoNode) return;
    
    if (estaVazia(l)) {
        lista->head = novoNode;
        lista->tail = novoNode;
    } else {
        novoNode->next = lista->head;
        lista->head->prev = novoNode;
        lista->head = novoNode;
    }
    
    lista->tam++;
}

void insereTail(Lista *l, void *novaInfo) {
    struct Lista *lista = (struct Lista*)l;
    
    if (!lista || !novaInfo) {
        fprintf(stderr, "Erro: parâmetros inválidos em insereTail\n");
        return;
    }
    
    struct No *novoNode = criaNode(novaInfo);
    if (!novoNode) return;
    
    if (estaVazia(l)) {
        lista->head = novoNode;
        lista->tail = novoNode;
    } else {
        novoNode->prev = lista->tail;
        lista->tail->next = novoNode;
        lista->tail = novoNode;
    }
    
    lista->tam++;
}

void* removeHead(Lista *l) {
    struct Lista *lista = (struct Lista*)l;
    
    if (estaVazia(l)) {
        return NULL;
    }
    
    struct No *node_to_remove = lista->head;
    void *data = node_to_remove->data;
    
    if (lista->tam == 1) {
        lista->head = NULL;
        lista->tail = NULL;
    } else {
        lista->head = node_to_remove->next;
        lista->head->prev = NULL;
    }
    
    free(node_to_remove);
    lista->tam--;
    
    return data;
}

void* removeTail(Lista *l) {
    struct Lista *lista = (struct Lista*)l;
    
    if (estaVazia(l)) {
        return NULL;
    }
    
    struct No *node_to_remove = lista->tail;
    void *data = node_to_remove->data;
    
    if (lista->tam == 1) {
        lista->head = NULL;
        lista->tail = NULL;
    } else {
        lista->tail = node_to_remove->prev;
        lista->tail->next = NULL;
    }
    
    free(node_to_remove);
    lista->tam--;
    
    return data;
}

void* getHeadInfo(Lista *l) {
    struct Lista *lista = (struct Lista*)l;
    return (estaVazia(l)) ? NULL : lista->head->data;
}

void* getTailInfo(Lista *l) {
    struct Lista *lista = (struct Lista*)l;
    return (estaVazia(l)) ? NULL : lista->tail->data;
}

Node* getHeadNode(Lista *l) {
    struct Lista *lista = (struct Lista*)l;
    return (estaVazia(l)) ? NULL : (Node*)lista->head;
}

Node* getTailNode(Lista *l) {
    struct Lista *lista = (struct Lista*)l;
    return (estaVazia(l)) ? NULL : (Node*)lista->tail;
}

void* getNodeInfo(Node *n) {
    struct No *node = (struct No*)n;
    return (node == NULL) ? NULL : node->data;
}

Node* vaiNodeDepois(Node *n) {
    struct No *node = (struct No*)n;
    return (node == NULL) ? NULL : (Node*)node->next;
}

Node* vaiNodeAntes(Node *n) {
    struct No *node = (struct No*)n;
    return (node == NULL) ? NULL : (Node*)node->prev;
}

void* procuraLista(Lista *l, void *key, int (*compare)(void *a, void *b)) {
    struct Lista *lista = (struct Lista*)l;
    
    if (estaVazia(l) || !key || !compare) {
        return NULL;
    }
    
    struct No *current = lista->head;
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
    struct Lista *lista = (struct Lista*)l;
    
    if (estaVazia(l) || !predicate) {
        return 0;
    }
    
    int removed = 0;
    struct No *current = lista->head;
    struct No *next_node;
    
    while (current) {
        next_node = current->next;
        
        if (predicate(current->data)) {
            if (current->prev) {
                current->prev->next = current->next;
            } else {
                lista->head = current->next;
            }
            
            if (current->next) {
                current->next->prev = current->prev;
            } else {
                lista->tail = current->prev;
            }
            
            if (free_data) {
                free_data(current->data);
            }
            free(current);
            
            lista->tam--;
            removed++;
        }
        
        current = next_node;
    }
    
    return removed;
}

void limpaLista(Lista *l, void (*free_data)(void *data)) {
    struct Lista *lista = (struct Lista*)l;
    
    if (!lista) return;
    
    struct No *current = lista->head;
    struct No *next_node;
    
    while (current) {
        next_node = current->next;
        
        if (free_data) {
            free_data(current->data);
        }
        free(current);
        
        current = next_node;
    }
    
    lista->head = NULL;
    lista->tail = NULL;
    lista->tam = 0;
}

void freeLista(Lista *l, void (*free_data)(void *data)) {
    struct Lista *lista = (struct Lista*)l;
    
    if (!lista) return;
    
    limpaLista(l, free_data);
    free(lista);
}
