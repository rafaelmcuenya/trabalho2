#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"
#include "strdupi.h"

typedef struct avl_node {
    void* info;             
    struct avl_node* esq;   
    struct avl_node* dir;   
    int altura;            
} AVLNode;

struct avl_tree {
    AVLNode* raiz;        
    int tamanho;         
    int (*cmp)(void*, void*); 
};

static int max(int a, int b);
static int alturaNo(AVLNode* no);
static int fatorBalanceamento(AVLNode* no);
static AVLNode* rotacaoDireita(AVLNode* y);
static AVLNode* rotacaoEsquerda(AVLNode* x);
static AVLNode* inserirNo(AVLNode* no, void* info, int (*cmp)(void*, void*), int* inserido);
static AVLNode* menorValorNo(AVLNode* no);
static AVLNode* removerNo(AVLNode* raiz, void* chave, int (*cmp)(void*, void*), int* removido);
static AVLNode* buscarNo(AVLNode* no, void* chave, int (*cmp)(void*, void*));
static void percorrerOrdem(AVLNode* no, void (*printInfo)(void*));
static void liberarNos(AVLNode* no, void (*liberaInfo)(void*));
static int calcularAlturaArvore(AVLNode* no);

static int max(int a, int b) {
    return (a > b) ? a : b;
}

static int alturaNo(AVLNode* no) {
    return no ? no->altura : 0;
}

static int fatorBalanceamento(AVLNode* no) {
    return no ? alturaNo(no->esq) - alturaNo(no->dir) : 0;
}

static AVLNode* rotacaoDireita(AVLNode* y) {
    if (!y || !y->esq) return y;
    
    AVLNode* x = y->esq;
    AVLNode* T2 = x->dir;
    x->dir = y;
    y->esq = T2;
    y->altura = max(alturaNo(y->esq), alturaNo(y->dir)) + 1;
    x->altura = max(alturaNo(x->esq), alturaNo(x->dir)) + 1;
    
    return x;
}

static AVLNode* rotacaoEsquerda(AVLNode* x) {
    if (!x || !x->dir) return x;
    
    AVLNode* y = x->dir;
    AVLNode* T2 = y->esq;
    y->esq = x;
    x->dir = T2;
    x->altura = max(alturaNo(x->esq), alturaNo(x->dir)) + 1;
    y->altura = max(alturaNo(y->esq), alturaNo(y->dir)) + 1;
    
    return y;
}

static AVLNode* inserirNo(AVLNode* no, void* info, int (*cmp)(void*, void*), int* inserido) {
    if (!no) {
        AVLNode* novo = (AVLNode*)malloc(sizeof(AVLNode));
        if (!novo) {
            fprintf(stderr, "Erro: falha na alocação de nó AVL\n");
            *inserido = 0;
            return NULL;
        }
        
        novo->info = info;
        novo->esq = NULL;
        novo->dir = NULL;
        novo->altura = 1;
        *inserido = 1;
        return novo;
    }
    
    int comparacao = cmp(info, no->info);
    
    if (comparacao < 0) {
        no->esq = inserirNo(no->esq, info, cmp, inserido);
    } else if (comparacao > 0) {
        no->dir = inserirNo(no->dir, info, cmp, inserido);
    } else {
        *inserido = 0;
        return no;
    }
    
    no->altura = max(alturaNo(no->esq), alturaNo(no->dir)) + 1;
    int balance = fatorBalanceamento(no);
    
    // Esquerda-Esquerda
    if (balance > 1 && cmp(info, no->esq->info) < 0) {
        return rotacaoDireita(no);
    }
    
    // Direita-Direita
    if (balance < -1 && cmp(info, no->dir->info) > 0) {
        return rotacaoEsquerda(no);
    }
    
    // Esquerda-Direita
    if (balance > 1 && cmp(info, no->esq->info) > 0) {
        no->esq = rotacaoEsquerda(no->esq);
        return rotacaoDireita(no);
    }
    
    // Direita-Esquerda
    if (balance < -1 && cmp(info, no->dir->info) < 0) {
        no->dir = rotacaoDireita(no->dir);
        return rotacaoEsquerda(no);
    }
    
    return no;
}

static AVLNode* menorValorNo(AVLNode* no) {
    AVLNode* atual = no;
    while (atual && atual->esq) {
        atual = atual->esq;
    }
    return atual;
}

static AVLNode* removerNo(AVLNode* raiz, void* chave, int (*cmp)(void*, void*), int* removido) {
    if (!raiz) {
        *removido = 0;
        return NULL;
    }
    
    int comparacao = cmp(chave, raiz->info);
    
    if (comparacao < 0) {
        raiz->esq = removerNo(raiz->esq, chave, cmp, removido);
    } else if (comparacao > 0) {
        raiz->dir = removerNo(raiz->dir, chave, cmp, removido);
    } else {
        *removido = 1;
        
        if (!raiz->esq || !raiz->dir) {
            AVLNode* temp = raiz->esq ? raiz->esq : raiz->dir;
            
            if (!temp) {
                temp = raiz;
                raiz = NULL;
            } else {
                *raiz = *temp; 
            }
            
            free(temp);
        } else {
            AVLNode* temp = menorValorNo(raiz->dir);
            
            raiz->info = temp->info;
            int dummy;
            raiz->dir = removerNo(raiz->dir, temp->info, cmp, &dummy);
        }
    }
    
    if (!raiz) {
        return NULL;
    }
    
    raiz->altura = max(alturaNo(raiz->esq), alturaNo(raiz->dir)) + 1;
    int balance = fatorBalanceamento(raiz);
    
    // Esquerda-Esquerda
    if (balance > 1 && fatorBalanceamento(raiz->esq) >= 0) {
        return rotacaoDireita(raiz);
    }
    
    // Esquerda-Direita
    if (balance > 1 && fatorBalanceamento(raiz->esq) < 0) {
        raiz->esq = rotacaoEsquerda(raiz->esq);
        return rotacaoDireita(raiz);
    }
    
    // Direita-Direita
    if (balance < -1 && fatorBalanceamento(raiz->dir) <= 0) {
        return rotacaoEsquerda(raiz);
    }
    
    // Direita-Esquerda
    if (balance < -1 && fatorBalanceamento(raiz->dir) > 0) {
        raiz->dir = rotacaoDireita(raiz->dir);
        return rotacaoEsquerda(raiz);
    }
    
    return raiz;
}

static AVLNode* buscarNo(AVLNode* no, void* chave, int (*cmp)(void*, void*)) {
    if (!no) return NULL;
    
    int comparacao = cmp(chave, no->info);
    
    if (comparacao < 0) {
        return buscarNo(no->esq, chave, cmp);
    } else if (comparacao > 0) {
        return buscarNo(no->dir, chave, cmp);
    } else {
        return no;
    }
}

static void percorrerOrdem(AVLNode* no, void (*printInfo)(void*)) {
    if (!no) return;
    
    percorrerOrdem(no->esq, printInfo);
    if (printInfo) printInfo(no->info);
    percorrerOrdem(no->dir, printInfo);
}

static void liberarNos(AVLNode* no, void (*liberaInfo)(void*)) {
    if (!no) return;
    
    liberarNos(no->esq, liberaInfo);
    liberarNos(no->dir, liberaInfo);
    
    if (liberaInfo) {
        liberaInfo(no->info);
    }
    
    free(no);
}

static int calcularAlturaArvore(AVLNode* no) {
    if (!no) return 0;
    
    int alturaEsq = calcularAlturaArvore(no->esq);
    int alturaDir = calcularAlturaArvore(no->dir);
    
    return (alturaEsq > alturaDir ? alturaEsq : alturaDir) + 1;
}

Arvore* iniciarArvore(int (*cmp)(void*, void*)) {
    if (!cmp) {
        fprintf(stderr, "Erro: função de comparação não pode ser NULL\n");
        return NULL;
    }
    
    Arvore* t = (Arvore*)malloc(sizeof(Arvore));
    if (!t) {
        fprintf(stderr, "Erro: falha na alocação da árvore\n");
        return NULL;
    }
    
    t->raiz = NULL;
    t->tamanho = 0;
    t->cmp = cmp;
    
    return t;
}

bool vaziaAVL(Arvore* t) {
    return !t || t->tamanho == 0;
}

int getTamAVL(Arvore* t) {
    return t ? t->tamanho : 0;
}

int getAltura(Arvore* t) {
    if (!t || !t->raiz) return 0;
    return calcularAlturaArvore(t->raiz);
}

NoArv* getRaiz(Arvore* t) {
    return t ? (NoArv*)t->raiz : NULL;
}

NoArv* getNoEsquerda(NoArv* no) {
    AVLNode* n = (AVLNode*)no;
    return n ? (NoArv*)n->esq : NULL;
}

NoArv* getNoDireita(NoArv* no) {
    AVLNode* n = (AVLNode*)no;
    return n ? (NoArv*)n->dir : NULL;
}

void* getNoInfo(NoArv* no) {
    AVLNode* n = (AVLNode*)no;
    return n ? n->info : NULL;
}

NoArv* insereNo(Arvore* t, void* info) {
    if (!t || !info) {
        fprintf(stderr, "Erro: parâmetros inválidos em insereNo\n");
        return NULL;
    }
    
    int inserido = 0;
    t->raiz = inserirNo(t->raiz, info, t->cmp, &inserido);
    
    if (inserido) {
        t->tamanho++;
        return (NoArv*)buscarNo(t->raiz, info, t->cmp);
    }
    return NULL;
}

NoArv* removeNo(Arvore* t, void* chave) {
    if (!t || !chave) {
        fprintf(stderr, "Erro: parâmetros inválidos em removeNo\n");
        return NULL;
    }
    
    int removido = 0;
    AVLNode* noRemover = buscarNo(t->raiz, chave, t->cmp);
    if (!noRemover) {
        return NULL;
    }
    
    t->raiz = removerNo(t->raiz, chave, t->cmp, &removido);
    
    if (removido) {
        t->tamanho--;
    }
    return (NoArv*)noRemover;
}

NoArv* buscaBinaria(Arvore* t, void* chave) {
    if (!t || !chave) {
        return NULL;
    }
    return (NoArv*)buscarNo(t->raiz, chave, t->cmp);
}

void printOrdem(Arvore* t, void (*printInfo)(void*)) {
    if (!t || !printInfo) {
        fprintf(stderr, "Erro: parâmetros inválidos em printOrdem\n");
        return;
    }
    
    printf("Árvore (%d elementos): ", t->tamanho);
    percorrerOrdem(t->raiz, printInfo);
    printf("\n");
}

void freeArvore(Arvore* t, void (*liberaInfo)(void*)) {
    if (!t) return;
    
    liberarNos(t->raiz, liberaInfo);
    free(t);
}
