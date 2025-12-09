#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "strdupi.h"
#include "retangulo.h"

typedef struct{
    int id;
    double x, y;
    double largura, altura;
    char corB[8];
    char corP[8];
} RetanguloStruct;

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

int validaRetangulo(void* r){
    if (!r) return 0;
    RetanguloStruct* rect = (RetanguloStruct*)r;
    return (rect->id > 0) && (rect->largura > 0) && (rect->altura > 0) &&
           (rect->x == rect->x && rect->y == rect->y &&
            rect->largura == rect->largura && rect->altura == rect->altura) &&
           (!isinf(rect->x) && !isinf(rect->y) && !isinf(rect->largura) && !isinf(rect->altura)) &&
           (rect->corB[0] != '\0') && (rect->corP[0] != '\0');
}

Retangulo criaRetangulo(int i, double x, double y, double w, double h, char* corb, char* corp){
    if (i <= 0){
        fprintf(stderr, "Erro: ID do retângulo inválido\n");
        return NULL;
    }
    
    if (!corb || !corp){
        fprintf(stderr, "Erro: cores inválidas\n");
        return NULL;
    }
    
    if (w <= 0 || h <= 0){
        fprintf(stderr, "Erro: largura e/ou altura inválida(s)\n");
        return NULL;
    }
    
    if (x != x || y != y || isinf(x) || isinf(y) ||
        w != w || h != h || isinf(w) || isinf(h)){
        fprintf(stderr, "Erro: coordenadas e/ou dimensões inválida(s)\n");
        return NULL;
    }
    
    RetanguloStruct* r = (RetanguloStruct*)malloc(sizeof(RetanguloStruct));
    if (!r){
        fprintf(stderr, "Erro: falha na alocação do retângulo\n");
        return NULL;
    }
    
    r->id = i;
    r->x = x;
    r->y = y;
    r->largura = w;
    r->altura = h;
  
    preencherCor(r->corB, corb, "000000");
    preencherCor(r->corP, corp, "FFFFFF");
    
    return (Retangulo)r;
}

double areaRetangulo(Retangulo r){
    if (!r){
        fprintf(stderr, "Erro: retângulo NULL em areaRetangulo\n");
        return -1.0;
    }
    
    RetanguloStruct* rect = (RetanguloStruct*)r;
    return rect->largura * rect->altura;
}

int idRetangulo(Retangulo r){
    if (!r){
        fprintf(stderr, "Erro: retângulo NULL em idRetangulo\n");
        return -1;
    }
    RetanguloStruct* rect = (RetanguloStruct*)r;
    return rect->id;
}

void moveRetangulo(Retangulo r, double dx, double dy){
    if (!r){
        fprintf(stderr, "Erro: retângulo NULL em moveRetangulo\n");
        return;
    }
    
    if (dx != dx || dy != dy || isinf(dx) || isinf(dy)){
        fprintf(stderr, "Erro: parâmetros inválidos em moveRetangulo\n");
        return;
    }
    
    RetanguloStruct* rect = (RetanguloStruct*)r;
    double orig_x = rect->x;
    double orig_y = rect->y;
    rect->x += dx;
    rect->y += dy;
    
    if (rect->x != rect->x || rect->y != rect->y || isinf(rect->x) || isinf(rect->y)){
        fprintf(stderr, "Erro: coordenadas corrompidas após movimento\n");
        rect->x = orig_x;
        rect->y = orig_y;
    }
}

Retangulo clonaRetangulo(Retangulo r, int novoId){
    if (!r){
        fprintf(stderr, "Erro: tentativa de clonagem de retângulo NULL\n");
        return NULL;
    }
    
    if (novoId <= 0) {
        fprintf(stderr, "Erro: ID inválido para clone do retângulo\n");
        return NULL;
    }
    
    RetanguloStruct* rect = (RetanguloStruct*)r;
    Retangulo clone = criaRetangulo(novoId, rect->x, rect->y, rect->largura, rect->altura, rect->corB, rect->corP);
    
    if (!clone){
        fprintf(stderr, "Erro: falha ao clonar retângulo %d\n", rect->id);
    }
    return clone;
}

double getXRetangulo(Retangulo r){
    if (!r){
        fprintf(stderr, "Erro: retângulo NULL em getXRetangulo\n");
        return -1000.0;
    }
    
    RetanguloStruct* rect = (RetanguloStruct*)r;
    return rect->x;
}

double getYRetangulo(Retangulo r){
    if (!r){
        fprintf(stderr, "Erro: retângulo NULL em getYRetangulo\n");
        return -1000.0;
    }
    
    RetanguloStruct* rect = (RetanguloStruct*)r;
    return rect->y;
}

char* getCorBRetangulo(Retangulo r){
    if (!r){
        fprintf(stderr, "Erro: retângulo NULL em getCorBRetangulo\n");
        return NULL;
    }
    
    RetanguloStruct* rect = (RetanguloStruct*)r;
    char* temp = strdupi(rect->corB);
    
    if (!temp){
        fprintf(stderr, "Erro: falha na alocação da cor de borda\n");
    }
    return temp;
}

char* getCorPRetangulo(Retangulo r){
    if (!r){
        fprintf(stderr, "Erro: retângulo NULL em getCorPRetangulo\n");
        return NULL;
    }
    
    RetanguloStruct* rect = (RetanguloStruct*)r;
    char* temp = strdupi(rect->corP);
    
    if (!temp){
        fprintf(stderr, "Erro: falha na alocação da cor de preenchimento\n");
    }
    return temp;
}

double getLarguraRetangulo(Retangulo r){
    if (!r){
        fprintf(stderr, "Erro: retângulo NULL em getLarguraRetangulo\n");
        return -1.0;
    }
    RetanguloStruct* rect = (RetanguloStruct*)r;
    return rect->largura;
}

double getAlturaRetangulo(Retangulo r){
    if (!r){
        fprintf(stderr, "Erro: retângulo NULL em getAlturaRetangulo\n");
        return -1.0;
    }
    RetanguloStruct* rect = (RetanguloStruct*)r;
    return rect->altura;
}

void setCorBRetangulo(Retangulo r, char* novaCor){
    if (!r || !novaCor || strlen(novaCor) == 0) {
        fprintf(stderr, "Erro: parâmetros inválidos em setCorBRetangulo\n");
        return;
    }
    RetanguloStruct* rect = (RetanguloStruct*)r;
    preencherCor(rect->corB, novaCor, "000000");
}

void setCorPRetangulo(Retangulo r, char* novaCor){
    if (!r || !novaCor || strlen(novaCor) == 0) {
        fprintf(stderr, "Erro: parâmetros inválidos em setCorPRetangulo\n");
        return;
    }
    RetanguloStruct* rect = (RetanguloStruct*)r;
    preencherCor(rect->corP, novaCor, "FFFFFF");
}

void liberaRetangulo(Retangulo r){
    if (!r){
        fprintf(stderr, "Aviso: tentativa de liberar retângulo NULL\n");
        return;
    }
    RetanguloStruct* rect = (RetanguloStruct*)r;
    free(rect);
}
