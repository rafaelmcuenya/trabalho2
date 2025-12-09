#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "circulo.h"
#include "strdupi.h"
#include "anteparo.h"

typedef struct{
    int id;
    double x, y;
    double r;
    char corB[8];
    char corP[8];
} CirculoStruct;

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

int validaCirculo(void* circ){
    if (!circ) return 0;
    CirculoStruct* circle = (CirculoStruct*)circ;
  
    return (circle->id > 0) && 
           (circle->r > 0) && (circle->x == circle->x && circle->y == circle->y && circle->r == circle->r) &&
           (!isinf(circle->x) && !isinf(circle->y) && !isinf(circle->r)) &&
           (circle->corB[0] != '\0') && (circle->corP[0] != '\0');
}

Circulo criaCirculo(int i, double x, double y, double r, char* corb, char* corp){
    if (i <= 0){
        fprintf(stderr, "Erro: ID do círculo inválido\n");
        return NULL;
    }
    
    if (!corb || !corp){
        fprintf(stderr, "Erro: cor(es) do círculo inválida(s)\n");
        return NULL;
    }
    
    if (r <= 0){
        fprintf(stderr, "Erro: raio do círculo inválido\n");
        return NULL;
    }
    
    if (x != x || y != y || isinf(x) || isinf(y) ||
        r != r || isinf(r)){
        fprintf(stderr, "Erro: coordenada(s) e/ou raio inválido(s)\n");
        return NULL;
    }
    
    CirculoStruct* c = (CirculoStruct*)malloc(sizeof(CirculoStruct));
    if (!c){
        fprintf(stderr, "Erro: falha na alocação do círculo\n");
        return NULL;
    }
    
    c->id = i;
    c->x = x;
    c->y = y;
    c->r = r;
    
    preencherCor(c->corB, corb, "000000");
    preencherCor(c->corP, corp, "FFFFFF");
    
    return (Circulo)c;
}

double areaCirculo(Circulo c){
    if (!c){
        fprintf(stderr, "Erro: círculo NULL em areaCirculo\n");
        return -1.0;
    }
    CirculoStruct* circle = (CirculoStruct*)c;
    return circle->r * circle->r * 3.14;
}

int idCirculo(Circulo c){
    if (!c){
        fprintf(stderr, "Erro: círculo NULL em idCirculo\n");
        return -1;
    }
    CirculoStruct* circle = (CirculoStruct*)c;
    return circle->id;
}

void moveCirculo(Circulo c, double dx, double dy){
    if (!c){
        fprintf(stderr, "Erro: círculo NULL em moveCirculo\n");
        return;
    }
    
    if (dx != dx || dy != dy || isinf(dx) || isinf(dy)){
        fprintf(stderr, "Erro: parâmetros inválidos em moveCirculo\n");
        return;
    }
    
    CirculoStruct* circle = (CirculoStruct*)c;
    double orig_x = circle->x;
    double orig_y = circle->y;
    circle->x += dx;
    circle->y += dy;
  
    if (circle->x != circle->x || circle->y != circle->y || 
        isinf(circle->x) || isinf(circle->y)){
        fprintf(stderr, "Erro: coordenadas corrompidas após movimento\n");
        circle->x = orig_x;
        circle->y = orig_y;
    }
}

double getXCirculo(Circulo c){
    if (!c){
        fprintf(stderr, "Erro: círculo NULL em getXCirculo\n");
        return -1000.0;
    }
    
    CirculoStruct* circle = (CirculoStruct*)c;
    return circle->x;
}

double getYCirculo(Circulo c){
    if (!c){
        fprintf(stderr, "Erro: círculo NULL em getYCirculo\n");
        return -1000.0;
    }
    
    CirculoStruct* circle = (CirculoStruct*)c;
    return circle->y;
}

char* getCorBCirculo(Circulo c){
    if (!c){
        fprintf(stderr, "Erro: círculo NULL em getCorBCirculo\n");
        return NULL;
    }
    
    CirculoStruct* circle = (CirculoStruct*)c;
    char* copia = strdupi(circle->corB);
    
    if (!copia){
        fprintf(stderr, "Erro: falha na alocação da cor de borda\n");
    }
    return copia; 
}

char* getCorPCirculo(Circulo c){
    if (!c){
        fprintf(stderr, "Erro: círculo NULL em getCorPCirculo\n");
        return NULL;
    }
    
    CirculoStruct* circle = (CirculoStruct*)c;
    char* copia = strdupi(circle->corP);
    
    if (!copia){
        fprintf(stderr, "Erro: falha na alocação da cor de preenchimento\n");
    }
    return copia; 
}

double getRaioCirculo(Circulo c){
    if (!c){
        fprintf(stderr, "Erro: círculo NULL em getRaioCirculo\n");
        return -1.0;
    }
    
    CirculoStruct* circle = (CirculoStruct*)c;
    return circle->r;
}

void setCorBCirculo(Circulo c, char* novaCor){
    if (!c || !novaCor){
        fprintf(stderr, "Erro: parâmetros inválidos em setCorBCirculo\n");
        return;
    }
    CirculoStruct* circle = (CirculoStruct*)c;
    preencherCor(circle->corB, novaCor, "000000");
}

void setCorPCirculo(Circulo c, char* novaCor){
    if (!c || !novaCor){
        fprintf(stderr, "Erro: parâmetros inválidos em setCorPCirculo\n");
        return;
    }
    CirculoStruct* circle = (CirculoStruct*)c;
    preencherCor(circle->corP, novaCor, "FFFFFF");
}

Circulo clonaCirculo(Circulo c){
    if (!c){
        fprintf(stderr, "Erro: tentativa de clonar círculo NULL\n");
        return NULL;
    }
    
    CirculoStruct* circle = (CirculoStruct*)c;
    int novoId = circle->id * 1000 + (int)(circle->x + circle->y + circle->r);
    if (novoId <= circle->id) novoId = circle->id + 1000;
    Circulo clone = criaCirculo(novoId, circle->x, circle->y, circle->r, circle->corB, circle->corP);
    
    if (!clone){
        fprintf(stderr, "Erro: falha ao clonar círculo %d\n", circle->id);
    }
    return clone;
}

Anteparo* criarAnteparoC(Circulo* circulo, char direcao, int novo_id) {
    if (!circulo || (direcao != 'h' && direcao != 'v')) {
        return NULL;
    }

    Anteparo* ant = (Anteparo*)malloc(sizeof(Anteparo));
    if (!seg) return NULL;

    seg->id = novo_id;
    seg->cor = circle->cor_borda;

    double x = circle->ancora.x;
    double y = circle->ancora.y;
    double r = circle->raio;

    if (direcao == 'h') {
        ant->p1.x = x - r;
        ant->p1.y = y;
        ant->p2.x = x + r;
        ant->p2.y = y;
    } else {
        ant->p1.x = x;
        ant->p1.y = y - r;
        ant->p2.x = x;
        ant->p2.y = y + r;
    }

    return ant;
}

void liberaCirculo(Circulo c){
    if (!c){
        fprintf(stderr, "Erro: tentativa de liberar círculo NULL\n");
        return;
    }
    CirculoStruct* circle = (CirculoStruct*)c;
    free(circle);
}
