#include <stdlib.h>
#include <math.h>
#include "ponto.h"

typedef struct {
    double x;
    double y;
} PontoStruct;

int validaPonto(double x, double y) {
    return (x >= 0 && y >= 0);
}

Ponto criaPonto(double x, double y) {
    if (!validaPonto(x, y)) {
        return NULL;
    }
    
    PontoStruct* p = (PontoStruct*)malloc(sizeof(PontoStruct));
    if (p == NULL) {
        return NULL;
    }
    
    p->x = x;
    p->y = y;
    return (Ponto)p;
}

void movePonto(Ponto p, double x, double y) {
    if (p != NULL && validaPonto(x, y)) {
        PontoStruct* ponto = (PontoStruct*)p;  
        ponto->x = x;
        ponto->y = y;
    }
}

double getXPonto(Ponto p) {
    if (p == NULL) {
        return -1; 
    }
    PontoStruct* ponto = (PontoStruct*)p;  
    return ponto->x;
}

double getYPonto(Ponto p) {
    if (p == NULL) {
        return -1;
    }
    PontoStruct* ponto = (PontoStruct*)p;  
    return ponto->y;
}

Ponto clonaPonto(Ponto p) {
    if (p == NULL) {
        return NULL;
    }
    PontoStruct* ponto = (PontoStruct*)p;  
    return criaPonto(ponto->x, ponto->y);
}

void liberaPonto(Ponto p) {
    if (p != NULL) {
        PontoStruct* ponto = (PontoStruct*)p;  
        free(ponto);
    }
}
