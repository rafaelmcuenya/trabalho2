#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "anteparo.h"
#include "strdupi.h"

typedef struct {
    int id;
    double x1, y1;
    double x2, y2;
    char* cor;  
} AnteparoStruct;

int validaAnteparo(void* a) {
    if (!a) return 0;
    AnteparoStruct* antep = (AnteparoStruct*)a;
    
    int pontosIguais = (antep->x1 == antep->x2 && antep->y1 == antep->y2);
    
    return (antep->id > 0) && (!pontosIguais) && (antep->cor != NULL && strlen(antep->cor) > 0) && 
           (antep->x1 == antep->x1 && antep->y1 == antep->y1 && antep->x2 == antep->x2 && antep->y2 == antep->y2) &&
           (!isinf(antep->x1) && !isinf(antep->y1) && !isinf(antep->x2) && !isinf(antep->y2));
}

Anteparo criaAnteparo(int id, double x1, double y1, double x2, double y2, char* cor) {
    if (id <= 0) {
        fprintf(stderr, "Erro: ID do anteparo deve ser positivo\n");
        return NULL;
    }
    
    if (!cor) {
        fprintf(stderr, "Erro: cor do anteparo não pode ser NULL\n");
        return NULL;
    }
    
    if (x1 == x2 && y1 == y2) {
        fprintf(stderr, "Erro: anteparo não pode ter ambos os pontos iguais\n");
        return NULL;
    }
    
    if (x1 != x1 || y1 != y1 || x2 != x2 || y2 != y2 || 
        isinf(x1) || isinf(y1) || isinf(x2) || isinf(y2)) {
        fprintf(stderr, "Erro: coordenadas do anteparo inválidas\n");
        return NULL;
    }
    
    AnteparoStruct* a = (AnteparoStruct*)malloc(sizeof(AnteparoStruct));
    if (!a) {
        fprintf(stderr, "Erro: falha na alocação do anteparo\n");
        return NULL;
    }
    
    a->id = id;
    a->x1 = x1;
    a->y1 = y1;
    a->x2 = x2;
    a->y2 = y2;
    
    if (cor[0] == '#') {
        a->cor = strdupi(cor);
    } else {
        char temp[20];
        snprintf(temp, sizeof(temp), "#%s", cor);
        a->cor = strdupi(temp);
    }
    
    if (!a->cor) {
        fprintf(stderr, "Erro: falha na alocação da cor do anteparo\n");
        free(a);
        return NULL;
    }
    
    return (Anteparo)a;
}

int idAnteparo(Anteparo a) {
    if (!a) {
        fprintf(stderr, "Erro: anteparo NULL em idAnteparo\n");
        return -1;
    }
    AnteparoStruct* antep = (AnteparoStruct*)a;
    return antep->id;
}

double getX1Anteparo(Anteparo a) {
    if (!a) {
        fprintf(stderr, "Erro: anteparo NULL em getX1Anteparo\n");
        return -1.0;
    }
    AnteparoStruct* antep = (AnteparoStruct*)a;
    return antep->x1;
}

double getY1Anteparo(Anteparo a) {
    if (!a) {
        fprintf(stderr, "Erro: anteparo NULL em getY1Anteparo\n");
        return -1.0;
    }
    AnteparoStruct* antep = (AnteparoStruct*)a;
    return antep->y1;
}

double getX2Anteparo(Anteparo a) {
    if (!a) {
        fprintf(stderr, "Erro: anteparo NULL em getX2Anteparo\n");
        return -1.0;
    }
    AnteparoStruct* antep = (AnteparoStruct*)a;
    return antep->x2;
}

double getY2Anteparo(Anteparo a) {
    if (!a) {
        fprintf(stderr, "Erro: anteparo NULL em getY2Anteparo\n");
        return -1.0;
    }
    AnteparoStruct* antep = (AnteparoStruct*)a;
    return antep->y2;
}

char* getCorAnteparo(Anteparo a) {
    if (!a) {
        fprintf(stderr, "Erro: anteparo NULL em getCorAnteparo\n");
        return NULL;
    }
    AnteparoStruct* antep = (AnteparoStruct*)a;
    
    char* copia = strdupi(antep->cor);
    if (!copia) {
        fprintf(stderr, "Erro: falha na alocação da cópia da cor\n");
        return NULL;
    }
    return copia;
}

void moveAnteparo(Anteparo a, double dx, double dy) {
    if (!a) {
        fprintf(stderr, "Erro: anteparo NULL em moveAnteparo\n");
        return;
    }
    
    if (dx != dx || dy != dy || isinf(dx) || isinf(dy)) {
        fprintf(stderr, "Erro: deltas inválidos em moveAnteparo\n");
        return;
    }
    
    AnteparoStruct* antep = (AnteparoStruct*)a;
    double orig_x1 = antep->x1;
    double orig_y1 = antep->y1;
    double orig_x2 = antep->x2;
    double orig_y2 = antep->y2;
    
    antep->x1 += dx;
    antep->y1 += dy;
    antep->x2 += dx;
    antep->y2 += dy;
    
    if (antep->x1 != antep->x1 || antep->y1 != antep->y1 || 
        antep->x2 != antep->x2 || antep->y2 != antep->y2 ||
        isinf(antep->x1) || isinf(antep->y1) || 
        isinf(antep->x2) || isinf(antep->y2)) {
        fprintf(stderr, "Erro: coordenadas corrompidas após movimento\n");
        antep->x1 = orig_x1;
        antep->y1 = orig_y1;
        antep->x2 = orig_x2;
        antep->y2 = orig_y2;
    }
}

Anteparo clonaAnteparo(Anteparo a, int novoId) {
    if (!a) {
        fprintf(stderr, "Erro: anteparo NULL em clonaAnteparo\n");
        return NULL;
    }
    
    AnteparoStruct* antep = (AnteparoStruct*)a;
    
    if (novoId <= 0) {
        fprintf(stderr, "Erro: novo ID inválido para clonagem\n");
        return NULL;
    }
    
    Anteparo clone = criaAnteparo(novoId, antep->x1, antep->y1, 
                                  antep->x2, antep->y2, antep->cor);
    
    if (!clone) {
        fprintf(stderr, "Erro: falha ao clonar anteparo %d\n", antep->id);
    }
    return clone;
}

void setCorAnteparo(Anteparo a, char* novaCor) {
    if (!a || !novaCor) {
        fprintf(stderr, "Erro: parâmetros inválidos em setCorAnteparo\n");
        return;
    }
    AnteparoStruct* antep = (AnteparoStruct*)a;
    
    free(antep->cor);
    
    if (novaCor[0] == '#') {
        antep->cor = strdupi(novaCor);
    } else {
        char temp[20];
        snprintf(temp, sizeof(temp), "#%s", novaCor);
        antep->cor = strdupi(temp);
    }
    
    if (!antep->cor) {
        fprintf(stderr, "Erro: falha na alocação da nova cor\n");
        antep->cor = strdupi("#000000");
    }
}

void liberaAnteparo(Anteparo a) {
    if (!a) {
        fprintf(stderr, "Aviso: tentativa de liberar anteparo NULL\n");
        return;
    }
    AnteparoStruct* antep = (AnteparoStruct*)a;
    
    if (antep->cor) {
        free(antep->cor);
    }
    
    free(antep);
}
