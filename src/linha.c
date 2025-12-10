#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "linha.h"
#include "strdupi.h"

typedef struct{
    int id;
    double x1, y1, x2, y2;
    char cor[8];
} LinhaStruct;

static void preencherCor(char* dest, const char* fonte, const char* corPadrao){
    if (fonte && strlen(fonte) > 0){
        const char* cor = (fonte[0] == '#') ? fonte + 1 : fonte;
        int len = strlen(cor);
        
        if (len == 6){
            strncpy(dest, cor, 6);
        } else if (len < 6){
            int zeros = 6 - len;
            for (int i = 0; i < zeros; i++){
                dest[i] = '0';
            }
            strncpy(dest + zeros, cor, len);
        } else{
            strncpy(dest, cor, 6);
        }
        dest[6] = '\0';
    } else{
        strcpy(dest, corPadrao);
    }
}

int validaLinha(void* l){
    if (!l) return 0;
    LinhaStruct* linha = (LinhaStruct*)l;

    return (linha->id > 0) && 
           (linha->cor[0] != '\0') &&  (linha->x1 == linha->x1 && linha->y1 == linha->y1 && 
            linha->x2 == linha->x2 && linha->y2 == linha->y2) &&
           (!isinf(linha->x1) && !isinf(linha->y1) &&  !isinf(linha->x2) && !isinf(linha->y2));
}

Linha criaLinha(int i, double x1, double y1, double x2, double y2, char* cor){
    if (i <= 0){
        fprintf(stderr, "Erro: ID da linha deve ser positivo\n");
        return NULL;
    }
    
    if (!cor){
        fprintf(stderr, "Erro: cor da linha não pode ser NULL\n");
        return NULL;
    }
    
    if (x1 != x1 || y1 != y1 || x2 != x2 || y2 != y2 || 
        isinf(x1) || isinf(y1) || isinf(x2) || isinf(y2)){
        fprintf(stderr, "Erro: coordenadas da linha inválidas\n");
        return NULL;
    }
    
    LinhaStruct* l = (LinhaStruct*)malloc(sizeof(LinhaStruct));
    if (!l){
        fprintf(stderr, "Erro: falha na alocação da linha\n");
        return NULL;
    }
    
    l->id = i;
    l->x1 = x1;
    l->y1 = y1;
    l->x2 = x2;
    l->y2 = y2;
    
    preencherCor(l->cor, cor, "000000");
    
    return (Linha)l;
}

double areaLinha(Linha l){
    if (!l){
        fprintf(stderr, "Erro: linha NULL em areaLinha\n");
        return -1.0;
    }
    
    LinhaStruct* linha = (LinhaStruct*)l;
    double dx = linha->x2 - linha->x1;
    double dy = linha->y2 - linha->y1;
    double comp = sqrt(dx * dx + dy * dy);
    return 2.0 * comp; 
}

int idLinha(Linha l){
    if (!l){
        fprintf(stderr, "Erro: linha NULL em idLinha\n");
        return -1;
    }
    LinhaStruct* linha = (LinhaStruct*)l;
    return linha->id;
}

void moveLinha(Linha l, double dx, double dy){
    if (!l){
        fprintf(stderr, "Erro: linha NULL em moveLinha\n");
        return;
    }
    
    if (dx != dx || dy != dy || isinf(dx) || isinf(dy)){
        fprintf(stderr, "Erro: deltas inválidos em moveLinha\n");
        return;
    }
    
    LinhaStruct* linha = (LinhaStruct*)l;
    double orig_x1 = linha->x1;
    double orig_y1 = linha->y1;
    double orig_x2 = linha->x2;
    double orig_y2 = linha->y2;
    linha->x1 += dx;
    linha->y1 += dy;
    linha->x2 += dx;
    linha->y2 += dy;
    
    if (linha->x1 != linha->x1 || linha->y1 != linha->y1 || 
        linha->x2 != linha->x2 || linha->y2 != linha->y2 ||
        isinf(linha->x1) || isinf(linha->y1) || 
        isinf(linha->x2) || isinf(linha->y2)){
        fprintf(stderr, "Erro: coordenadas corrompidas após movimento\n");
        linha->x1 = orig_x1;
        linha->y1 = orig_y1;
        linha->x2 = orig_x2;
        linha->y2 = orig_y2;
    }
}

Linha clonaLinha(Linha l, int novoId){
    if (!l){
        fprintf(stderr, "Erro: linha NULL em clonaLinha\n");
        return NULL;
    }
    
    if (novoId <= 0) {
        fprintf(stderr, "Erro: ID inválido para clone da linha\n");
        return NULL;
    }
    
    LinhaStruct* linha = (LinhaStruct*)l;
    Linha clone = criaLinha(novoId, linha->x1, linha->y1, linha->x2, linha->y2, linha->cor);
    
    if (!clone){
        fprintf(stderr, "Erro: falha ao clonar linha %d\n", linha->id);
    }
    return clone;
}

double getX1Linha(Linha l){
    if (!l){
        fprintf(stderr, "Erro: linha NULL em getX1Linha\n");
        return -1.0;
    }
    LinhaStruct* linha = (LinhaStruct*)l;
    return linha->x1;
}

double getY1Linha(Linha l){
    if (!l){
        fprintf(stderr, "Erro: linha NULL em getY1Linha\n");
        return -1.0;
    }
    LinhaStruct* linha = (LinhaStruct*)l;
    return linha->y1;
}

double getX2Linha(Linha l){
    if (!l){
        fprintf(stderr, "Erro: linha NULL em getX2Linha\n");
        return -1.0;
    }
    LinhaStruct* linha = (LinhaStruct*)l;
    return linha->x2;
}

double getY2Linha(Linha l){
    if (!l){
        fprintf(stderr, "Erro: linha NULL em getY2Linha\n");
        return -1.0;
    }
    LinhaStruct* linha = (LinhaStruct*)l;
    return linha->y2;
}

char* getCorLinha(Linha l){
    if (!l){
        fprintf(stderr, "Erro: linha NULL em getCorLinha\n");
        return NULL;
    }
    LinhaStruct* linha = (LinhaStruct*)l;

    char* copia = strdupi(linha->cor);
    if (!copia){
        fprintf(stderr, "Erro: falha na alocação da cor\n");
        return NULL;
    }
    return copia;
}

char* getCorBLinha(Linha l) {
    return getCorLinha(l);
}

char* getCorPLinha(Linha l) { 
    return getCorLinha(l);
}

void setCorLinha(Linha l, char* novaCor){
    if (!l || !novaCor){
        fprintf(stderr, "Erro: parâmetros inválidos em setCorLinha\n");
        return;
    }
    LinhaStruct* linha = (LinhaStruct*)l;
    preencherCor(linha->cor, novaCor, "000000");
}

void liberaLinha(Linha l){
    if (!l){
        fprintf(stderr, "Aviso: tentativa de liberar linha NULL\n");
        return;
    }
    LinhaStruct* linha = (LinhaStruct*)l;
    free(linha);
}
