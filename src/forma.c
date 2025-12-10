#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "forma.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"
#include "anteparo.h"
#include "strdupi.h"

typedef struct{
    TipoForma tipo;
    void* elemento;
} FormStruct;

int validaCirculo(void* circ);
int validaRetangulo(void* ret);
int validaLinha(void* lin);
int validaTexto(void* txt);

Forma criaForma(TipoForma tipo, void* elemento){
    if (!elemento){
        fprintf(stderr, "Erro: elemento NULL na criação da forma\n");
        return NULL;
    }
    
    if (tipo < Tc || tipo > Ta){
        fprintf(stderr, "Erro: tipo de forma inválido: %d\n", tipo);
        return NULL;
    }
    
    int elementoValido = 0;
    switch (tipo){
        case Tc: elementoValido = validaCirculo(elemento); break;
        case Tr: elementoValido = validaRetangulo(elemento); break;
        case Tl: elementoValido = validaLinha(elemento); break;
        case Tt: elementoValido = validaTexto(elemento); break;
        case Ta: elementoValido = validaAnteparo(elemento); break;
        default: elementoValido = 0;
    }
    
    if (!elementoValido){
        fprintf(stderr, "Erro: elemento inválido para o tipo %d\n", tipo);
        return NULL;
    }
    
    FormStruct* f = (FormStruct*)malloc(sizeof(FormStruct));
    if (!f){
        fprintf(stderr, "Erro: falha na alocação de memória para Forma\n");
        return NULL;
    }
    
    f->tipo = tipo;
    f->elemento = elemento;
    return (Forma)f;
}

TipoForma getTipoForma(Forma f){
    if (!f){
        fprintf(stderr, "Erro: forma NULL em getTipoForma\n");
        return -1; 
    }
    
    FormStruct* form = (FormStruct*)f;
    return form->tipo;
}

int getIdForma(Forma f){
    if (!f){
        fprintf(stderr, "Erro: forma NULL em getIdForma\n");
        return -1; 
    }
    
    FormStruct* form = (FormStruct*)f;

    if (!form->elemento){
        fprintf(stderr, "Erro: elemento NULL na forma\n");
        return -1;
    }
    
    switch (form->tipo){
        case Tc: return idCirculo(form->elemento);
        case Tr: return idRetangulo(form->elemento);
        case Tl: return idLinha(form->elemento);
        case Tt: return idTexto(form->elemento);
        case Ta: return idAnteparo(form->elemento);
        default:
            fprintf(stderr, "Erro: tipo de forma desconhecido em getIdForma: %d\n", form->tipo);
            return -1;
    }
}

double areaForma(Forma f){
    if (!f){
        fprintf(stderr, "Erro: forma NULL em areaForma\n");
        return -1.0; 
    }
    
    FormStruct* form = (FormStruct*)f;
    
    if (!form->elemento){
        fprintf(stderr, "Erro: elemento NULL na forma em areaForma\n");
        return -1.0;
    }
    
    switch (form->tipo){
        case Tc: return areaCirculo(form->elemento);
        case Tr: return areaRetangulo(form->elemento);
        case Tl: return areaLinha(form->elemento);
        case Tt: return areaTexto(form->elemento);
        case Ta: return 0.0;
        default:
            fprintf(stderr, "Erro: tipo de forma desconhecido em areaForma: %d\n", form->tipo);
            return -1.0;
    }
}

void moveForma(Forma f, double dx, double dy){
    if (!f){
        fprintf(stderr, "Erro: forma NULL em moveForma\n");
        return;
    }
    
    FormStruct* form = (FormStruct*)f;
    
    if (!form->elemento){
        fprintf(stderr, "Erro: elemento NULL na forma em moveForma\n");
        return;
    }
    
    if (dx != dx || dy != dy){ 
        fprintf(stderr, "Erro: deltas inválidos em moveForma: %f, %f\n", dx, dy);
        return;
    }
    
    switch (form->tipo){
        case Tc: moveCirculo(form->elemento, dx, dy); break;
        case Tr: moveRetangulo(form->elemento, dx, dy); break;
        case Tl: moveLinha(form->elemento, dx, dy); break;
        case Tt: moveTexto(form->elemento, dx, dy); break;
        case Ta: moveAnteparo(form->elemento, dx, dy); break;
        default:
            fprintf(stderr, "Erro: tipo de forma desconhecido em moveForma: %d\n", form->tipo);
            break;
    }
}

double getXForma(Forma f){
    if (!f){
        fprintf(stderr, "Erro: forma NULL em getXForma\n");
        return -1.0; 
    }
    
    FormStruct* form = (FormStruct*)f;
    
    if (!form->elemento){
        fprintf(stderr, "Erro: elemento NULL na forma em getXForma\n");
        return -1.0;
    }
    
    switch (form->tipo){
        case Tc: return getXCirculo(form->elemento);
        case Tr: return getXRetangulo(form->elemento);
        case Tl: return getXLinha(form->elemento);
        case Tt: return getXTexto(form->elemento);
        case Ta: return getX1Anteparo(form->elemento);
        default:
            fprintf(stderr, "Erro: tipo de forma desconhecido em getXForma: %d\n", form->tipo);
            return -1.0;
    }
}

double getYForma(Forma f){
    if (!f){
        fprintf(stderr, "Erro: forma NULL em getYForma\n");
        return -1.0;
    }
    
    FormStruct* form = (FormStruct*)f;
    
    if (!form->elemento){
        fprintf(stderr, "Erro: elemento NULL na forma em getYForma\n");
        return -999999.0;
    }
    
    switch (form->tipo){
        case Tc: return getYCirculo(form->elemento);
        case Tr: return getYRetangulo(form->elemento);
        case Tl: return getYLinha(form->elemento);
        case Tt: return getYTexto(form->elemento);
        case Ta: return getY1Anteparo(form->elemento);
        default:
            fprintf(stderr, "Erro: tipo de forma desconhecido em getYForma: %d\n", form->tipo);
            return -999999.0;
    }
}

char* getCorBForma(Forma f){
    if (!f){
        fprintf(stderr, "Erro: forma NULL em getCorBForma\n");
        return NULL;
    }
    
    FormStruct* form = (FormStruct*)f;
    
    if (!form->elemento){
        fprintf(stderr, "Erro: elemento NULL na forma em getCorBForma\n");
        return NULL;
    }
    
    switch (form->tipo){
        case Tc: return getCorBCirculo(form->elemento);
        case Tr: return getCorBRetangulo(form->elemento);
        case Tl: return getCorLinha(form->elemento);
        case Tt: return getCorBTexto(form->elemento);
        case Ta: return getCorAnteparo(form->elemento);
        default:
            fprintf(stderr, "Erro: tipo de forma desconhecido em getCorBForma: %d\n", form->tipo);
            return NULL;
    }
}

char* getCorPForma(Forma f){
    if (!f){
        fprintf(stderr, "Erro: forma NULL em getCorPForma\n");
        return NULL;
    }
    
    FormStruct* form = (FormStruct*)f;
    
    if (!form->elemento){
        fprintf(stderr, "Erro: elemento NULL na forma em getCorPForma\n");
        return NULL;
    }
    
    switch (form->tipo){
        case Tc: return getCorPCirculo(form->elemento);
        case Tr: return getCorPRetangulo(form->elemento);
        case Tl: return getCorLinha(form->elemento);
        case Tt: return getCorPTexto(form->elemento);
        case Ta: return getCorAnteparo(form->elemento);
        default:
            fprintf(stderr, "Erro: tipo de forma desconhecido em getCorPForma: %d\n", form->tipo);
            return NULL;
    }
}

Circulo getCirculoFromForma(Forma f){
    if (!f){
        fprintf(stderr, "Erro: forma NULL em getCirculoFromForma\n");
        return NULL;
    }
    FormStruct* form = (FormStruct*)f;
    if (form->tipo != Tc){
        fprintf(stderr, "Erro: forma não é do tipo círculo\n");
        return NULL;
    }
    return (Circulo)form->elemento;
}

Retangulo getRetanguloFromForma(Forma f){
    if (!f){
        fprintf(stderr, "Erro: forma NULL em getRetanguloFromForma\n");
        return NULL;
    }
    FormStruct* form = (FormStruct*)f;
    if (form->tipo != Tr){
        fprintf(stderr, "Erro: forma não é do tipo retângulo\n");
        return NULL;
    }
    return (Retangulo)form->elemento;
}

Linha getLinhaFromForma(Forma f){
    if (!f){
        fprintf(stderr, "Erro: forma NULL em getLinhaFromForma\n");
        return NULL;
    }
    FormStruct* form = (FormStruct*)f;
    if (form->tipo != Tl){
        fprintf(stderr, "Erro: forma não é do tipo linha\n");
        return NULL;
    }
    return (Linha)form->elemento;
}

Texto getTextoFromForma(Forma f){
    if (!f){
        fprintf(stderr, "Erro: forma NULL em getTextoFromForma\n");
        return NULL;
    }
    FormStruct* form = (FormStruct*)f;
    if (form->tipo != Tt){
        fprintf(stderr, "Erro: forma não é do tipo texto\n");
        return NULL;
    }
    return (Texto)form->elemento;
}

Anteparo getAnteparoFromForma(Forma f){
    if (!f){
        fprintf(stderr, "Erro: forma NULL em getAnteparoFromForma\n");
        return NULL;
    }
    FormStruct* form = (FormStruct*)f;
    if (form->tipo != Ta){
        fprintf(stderr, "Erro: forma não é do tipo anteparo\n");
        return NULL;
    }
    return (Anteparo)form->elemento;
}

void freeForma(Forma f){
    if (!f){
        fprintf(stderr, "Aviso: tentativa de liberar forma NULL\n");
        return;
    }
    
    FormStruct* form = (FormStruct*)f;
    
    if (form->elemento){
        switch (form->tipo){
            case Tc: liberaCirculo(form->elemento); break;
            case Tr: liberaRetangulo(form->elemento); break;
            case Tl: liberaLinha(form->elemento); break;
            case Tt: liberaTexto(form->elemento); break;
            case Ta: liberaAnteparo(form->elemento); break;
            default:
                fprintf(stderr, "Erro: tipo de forma desconhecido em liberaForma: %d\n", form->tipo);
                break;
        }
    } else{
        fprintf(stderr, "Aviso: elemento NULL na forma ao liberar\n");
    }
    free(form);
}
